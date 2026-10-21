/*
 * A066498  Numbers k such that 3 divides phi(k).
 * https://oeis.org/A066498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 135, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066498: numbers k such that 3 divides φ(k). Computed by
   searching for the n‑th such integer. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_PRIME 200000   /* upper bound for prime sieve */

/* Prime table */
static uint32_t primes[MAX_PRIME];
static size_t primes_cnt = 0;
static bool primes_init = false;

static void init_primes(void)
{
    if (primes_init) return;
    primes_init = true;
    bool is_comp[MAX_PRIME + 1] = {false};
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (!is_comp[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                is_comp[j] = true;
        }
    }
    size_t idx = 0;
    for (int i = 2; i <= MAX_PRIME; ++i)
        if (!is_comp[i]) primes[idx++] = (uint32_t)i;
    primes_cnt = idx;
}

/* Returns true iff the 64‑bit integer x satisfies 3 | φ(x).  */
static bool has_condition(uint64_t x)
{
    if (x % 9 == 0) return true;         /* divisible by 3² */
    uint64_t t = x;
    size_t i = 0;
    while (i < primes_cnt && (uint64_t)primes[i] * primes[i] <= t) {
        uint32_t p = primes[i];
        if (t % p == 0) {
            if (p % 6 == 1) return true; /* prime ≡ 1 (mod 6) */
            while (t % p == 0) t /= p;
        }
        ++i;
    }
    if (t > 1) {                         /* remaining prime factor */
        if (t % 6 == 1) return true;
    }
    return false;
}

/* Returns the n‑th element of the sequence.  */
seq_t a(idx_t n)
{
    /* dynamic storage for computed terms */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static uint64_t last_checked = 2; /* smallest candidate */

    if (!seq) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    /* Ensure prime table is ready */
    if (!primes_init) init_primes();

    /* Generate terms until we have at least n terms */
    while (seq_len < n) {
        if (has_condition(last_checked)) {
            if (seq_len >= seq_cap) {
                seq_cap *= 2;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)last_checked;
        }
        ++last_checked;
    }
    return seq[n - 1];
}
