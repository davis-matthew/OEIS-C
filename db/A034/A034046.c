/*
 * A034046  Numbers that are both primitively and imprimitively represented by
 *          x^2+y^2+z^2.
 * https://oeis.org/A034046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   representable_by_three_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are both primitively and impro‑primtively represented
   by x^2+y^2+z^2.
   A term n occurs iff:
     * n is representable as a^2+b^2+c^2   (Legendre condition)
     * n is not squarefree                 (has a prime‑square divisor)
     * n % 4 != 0                          (not divisible by 4)
   The function returns the n‑th such integer (offset 1).  Each call is
   memoised and works for any seq_t/idx_t types defined by the build. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/representable_by_three_squares.h"

/* ------------------------------------------------------------ */
/* Smallest‑prime‑factor sieve (up to a fixed limit that covers all
   10 000 terms comfortably).                                       */
static const size_t SPF_MAX = 2000000;   /* > all needed candidates   */
static uint64_t *spf = NULL;            /* array of smallest prime factor */
static bool      spf_ready = false;

/* Build the SPF table once.                                           */
static void build_spf(void) {
    spf = (uint64_t*)malloc((SPF_MAX + 1) * sizeof(uint64_t));
    for (size_t i = 0; i <= SPF_MAX; ++i) spf[i] = i;
    for (size_t i = 2; i * i <= SPF_MAX; ++i) {
        if (spf[i] == i) {
            for (size_t j = i * i; j <= SPF_MAX; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
    spf_ready = true;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Test whether n is squarefree.  Uses the precomputed SPF table.     */
static bool is_squarefree(idx_t n) {
    if (n < 4) return true;          /* 1,2,3 are squarefree      */
    idx_t t = n;
    while (t > 1) {
        uint64_t p = spf[(size_t)t];
        int cnt = 0;
        while (t % p == 0) {
            ++cnt;
            t /= p;
            if (cnt >= 2) return false;   /* p^2 | n */
        }
    }
    return true;
}

/* ------------------------------------------------------------ */
/* Sequence storage (memoised).                                      */
static seq_t *seq = NULL;            /* array of sequence values           */
static size_t  seq_len = 0;           /* number of values currently stored */
static size_t  seq_cap = 0;           /* allocated capacity                 */
static idx_t   next_candidate = 1;     /* next integer to test              */

/* Ensure that the sequence contains at least 'need' terms.           */
static void ensure_sequence_length(idx_t need) {
    if (seq_len >= need) return;
    /* Allocate if necessary.                                            */
    if (seq_cap == 0) {
        seq_cap = 1024;
        seq = (seq_t*)malloc(seq_cap * sizeof(seq_t));
    }
    /* Scan integers until we have enough terms.                          */
    while (seq_len < need) {
        idx_t cur = next_candidate++;
        /* Check the three required conditions.                           */
        if (!representable_by_three_squares(cur)) continue;
        if (cur % 4 == 0)                 continue;
        if (is_squarefree(cur))           continue;
        /* Store the term.                                                */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = (seq_t)cur;
    }
}

/* ------------------------------------------------------------ */
/* Public function.  Returns a(n), offset 1.                       */
seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* safety for n < 1            */
    if (!spf_ready) build_spf();            /* initialise sieve once       */
    ensure_sequence_length(n);
    return seq[n - 1];
}
