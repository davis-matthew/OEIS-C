/*
 * A147627  Numbers n = concat(a,b) such that bigomega(n) = bigomega(a) *
 *          bigomega(b), where bigomega = A001222.
 * https://oeis.org/A147627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the nth positive integer satisfying the above property
 * Range:     u8 n <= 52, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   uint64_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A147627:  Numbers n = concat(a,b) such that bigomega(n)=bigomega(a)*bigomega(b)  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/uint64_digits.h"

/* ---------- bigomega sieve (smallest prime factor + Ω) ---------- */
static uint32_t *spf_arr = NULL;      /* smallest prime factor              */
static uint8_t  *omega_arr = NULL;    /* Ω(n) for 0..cap                    */
static size_t   omega_cap = 0;        /* current size of arrays             */

/* grow the sieve up to at least 'need' (inclusive) */
static void ensure_omega_limit(size_t need) {
    if (need <= omega_cap) return;

    size_t new_cap = omega_cap ? omega_cap * 2 : 1024;
    if (new_cap < need) new_cap = need;

    uint32_t *new_spf = (uint32_t *)realloc(spf_arr, (new_cap + 1) * sizeof(uint32_t));
    uint8_t  *new_omega = (uint8_t  *)realloc(omega_arr, (new_cap + 1) * sizeof(uint8_t));
    if (!new_spf || !new_omega) abort();

    /* zero new entries */
    for (size_t i = omega_cap + 1; i <= new_cap; ++i) new_spf[i] = 0;
    new_spf[0] = new_spf[1] = 0;
    for (size_t i = omega_cap + 1; i <= new_cap; ++i) new_omega[i] = 0;

    spf_arr = new_spf;
    omega_arr = new_omega;

    /* sieve for smallest prime factor */
    for (size_t i = 2; i <= new_cap; ++i) {
        if (spf_arr[i] == 0) {               /* i is prime */
            spf_arr[i] = (uint32_t)i;
            if (i * i <= new_cap) {
                for (size_t j = i * i; j <= new_cap; j += i) {
                    if (spf_arr[j] == 0) spf_arr[j] = (uint32_t)i;
                }
            }
        }
    }

    /* compute Ω using spf */
    for (size_t i = 2; i <= new_cap; ++i) {
        omega_arr[i] = 1 + omega_arr[i / spf_arr[i]];
    }

    omega_cap = new_cap;
}

/* ---------- sequence storage ---------- */
static seq_t *seq_arr = NULL;   /* contains the found terms */
static size_t seq_cap = 0;
static size_t seq_len = 0;

/* add a term to the sequence array, growing if necessary */
static void add_term(seq_t val) {
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_arr = tmp;
        seq_cap = new_cap;
    }
    seq_arr[seq_len++] = val;
}

/* ---------- 10‑to‑10^19 pow10 table (uint64_t) ---------- */
static uint64_t pow10_tab[20] = {0};

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    static bool init = false;
    static idx_t current = 2;   /* next candidate to test */
    if (!init) {
        /* build pow10 table */
        pow10_tab[0] = 1;
        for (int i = 1; i < 20; ++i) pow10_tab[i] = pow10_tab[i-1] * 10ULL;

        /* seed the sieve */
        ensure_omega_limit(1024);

        init = true;
    }

    /* generate terms until we have n of them */
    while (seq_len < n) {
        ensure_omega_limit((size_t)current);

        int d = uint64_digits((uint64_t)current);
        bool ok = false;
        for (int pos = 1; pos < d && !ok; ++pos) {
            uint64_t power = pow10_tab[d - pos];
            uint64_t a_part = current / power;
            uint64_t b_part = current % power;

            /* check that b_part has the required number of digits (no leading zeros) */
            if (uint64_digits(b_part) != d - pos) continue;

            uint8_t oa = omega_arr[(size_t)a_part];
            uint8_t ob = omega_arr[(size_t)b_part];
            uint8_t om = omega_arr[(size_t)current];
            if ((uint64_t)oa * (uint64_t)ob == (uint64_t)om) ok = true;
        }

        if (ok) add_term((seq_t)current);
        current++;
    }

    return seq_arr[n - 1];
}
