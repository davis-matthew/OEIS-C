/*
 * A182155  Integers of the form: 0/3 + 1/3 + 2/3 + 3/3 + 5/3 + 7/3 + 11/3 +
 *          13/3 + 17/3 + ....
 * https://oeis.org/A182155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A182155:  a(n) is the n-th integer obtained by taking the sum of the
   non‑negative non‑composite numbers (0, 1, 2, 3, 5, 7, …), dividing
   that running total by 3, and recording the quotient whenever the total
   is divisible by 3.  The function generates the terms sequentially,
   keeping a cache so that any call can be answered in O(1) after a
   single pass up through the requested index. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *values = NULL;          /* cached sequence values   */
static size_t values_len = 0;         /* number of cached values  */
static size_t values_cap = 0;         /* capacity of values[]     */

static uint64_t *primes = NULL;      /* list of primes seen so far */
static size_t primes_len = 0;         /* number of primes stored   */
static size_t primes_cap = 0;         /* capacity of primes[]     */

static seq_t sum = 0;                 /* running sum of numerators */
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/*--- helper to grow the values array ----------------------------------*/
static void add_value(seq_t v)
{
    if (values_len == values_cap) {
        values_cap = values_cap ? values_cap * 2 : 16;
        values = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
    }
    values[values_len++] = v;
}

/*--- helper to grow the primes array ----------------------------------*/
static void add_prime(uint64_t p)
{
    if (primes_len == primes_cap) {
        primes_cap = primes_cap ? primes_cap * 2 : 16;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    primes[primes_len++] = p;
}

/*--- ensure we have computed at least n terms --------------------------*/
static void ensure_sequence(idx_t n)
{
    if (values_len >= n) return;          /* already computed */

    /* First call: initialise the very first two non‑composite numbers */
    if (values_len == 0) {
        sum = 0;                           /* current sum */
        next_candidate = 2;                 /* first prime to test */
        add_value((seq_t)0);                /* 0/3 = 0 contributes a(1)=0 */
        sum += (seq_t)1;                    /* add the number 1 (does not record) */
    }

    /* Generate primes and accumulate the sum until we have n values */
    while (values_len < (size_t)n) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p > next_candidate / p) break;   /* avoid overflow */
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            add_prime(next_candidate);
            sum += (seq_t)next_candidate;
            if ((sum % (seq_t)3) == 0) {
                seq_t term = sum / 3;
                add_value(term);
            }
        }
        ++next_candidate;                     /* advance to next integer */
    }
}

/*--- public sequence function ------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for 0, but safe */
    ensure_sequence(n);
    return values[n - 1];                   /* offset 1 */
}
