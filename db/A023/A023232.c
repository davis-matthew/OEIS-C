/*
 * A023232  Primes p such that 8*p + 9 is also prime.
 * https://oeis.org/A023232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A023232: primes p such that 8*p + 9 is also prime.
   a(n) returns the n‑th such prime (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

static seq_t *prime_seq = NULL;   /* stored terms */
static size_t prime_cnt = 0;      /* number of terms produced */
static size_t prime_cap = 0;      /* array capacity */
static idx_t last_checked = 2;    /* highest candidate examined */

/* Ensure capacity for more terms. */
static void grow_prime_seq(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 128;
    seq_t *tmp = (seq_t *)realloc(prime_seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();           /* allocation failure */
    prime_seq = tmp;
    prime_cap = new_cap;
}

/* Find the n‑th term, with memoisation for arbitrary call order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety: offset is 1 */

    /* Produce terms until we have at least n of them. */
    while (prime_cnt < n) {
        idx_t candidate = last_checked;

        /* advance to the next odd number ≥ 3 */
        if (candidate < 3) candidate = 3;
        if ((candidate & 1) == 0) candidate++;

        for (;;) {               /* loop until a suitable prime is found */
            if (is_prime(candidate)) {
                idx_t check = candidate * 8 + 9;
                if (is_prime(check)) {
                    /* store the result */
                    if (prime_cnt == prime_cap) grow_prime_seq();
                    prime_seq[prime_cnt++] = (seq_t)candidate;
                    /* move past this candidate for next search */
                    last_checked = candidate + 1;
                    break;      /* ready for next term */
                }
            }
            /* try next odd candidate */
            candidate += 2;
        }
    }
    return prime_seq[n - 1];
}
