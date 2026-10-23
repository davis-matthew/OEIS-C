/*
 * A139897  Primes of the form 3*x^2+80*y^2.
 * https://oeis.org/A139897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 413, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th prime of the form 3*x^2+80*y^2 (equivalently,
   primes congruent to 83 or 107 modulo 120, plus the exceptional prime 3). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static bool initialized = false;
static seq_t *seq = NULL;     /* array of found primes */
static idx_t seq_len = 0;     /* number of primes stored */

static void init(void)
{
    if (initialized) return;

    /* Upper bound sufficient for the first 10,000 terms. */
    const idx_t LIMIT = 2500000;
    /* Bit array for sieve (0 = prime candidate, 1 = composite) */
    char *isComp = (char*)calloc((size_t)LIMIT + 1, 1);

    /* Simple Eratosthenes sieve */
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!isComp[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) {
                isComp[j] = 1;
            }
        }
    }

    /* Allocate array for results, starting with capacity 10,000. */
    size_t cap = 10000;
    seq = (seq_t*)malloc(cap * sizeof(seq_t));
    seq_len = 0;

    /* Collect primes that are 3 or ≡83 or 107 (mod 120). */
    for (idx_t p = 3; p <= LIMIT; ++p) {
        if (!isComp[p]) {
            if (p == 3 || p % 120 == 83 || p % 120 == 107) {
                if (seq_len >= cap) {
                    cap <<= 1;                  /* double capacity */
                    seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)p;
            }
        }
    }

    free(isComp);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n < 1, but avoid UB */
    init();
    if (n <= seq_len) {
        return seq[n - 1];
    }
    /* In the unlikely event that our limit was insufficient, return 0. */
    return 0;
}
