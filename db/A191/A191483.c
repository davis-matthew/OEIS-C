/*
 * A191483  Even discriminants of imaginary quadratic fields, negated.
 * https://oeis.org/A191483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*k, where k is the n-th positive integer that is *
 *            squarefree and congruent to 1 or 2 modulo 4. This generates the
 *            * negated even discriminants of imaginary quadratic fields
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is built on demand and cached for efficiency. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* The build system supplies the real types seq_t and idx_t. */

#define MAX_CACHE 200000      /* should hold more than 10000 terms */

static seq_t cache[MAX_CACHE];
static size_t cache_size = 0;
static idx_t next_k = 1;

/* Test whether the integer k is squarefree. */
static bool is_squarefree(idx_t k)
{
    for (idx_t p = 2; p * p <= k; ++p) {
        if (k % p == 0) {
            idx_t q = k / p;
            if (q % p == 0) {
                return false;   /* p^2 divides k */
            }
        }
    }
    return true;
}

/* Build sequence up to the requested index n. */
static void ensure_sequence(idx_t n)
{
    while (cache_size < n) {
        /* Find next valid k */
        for (;;) {
            idx_t k = next_k++;
            if ((k & 3) == 1 || (k & 3) == 2) { /* k % 4 == 1 or 2 */
                if (is_squarefree(k)) {
                    if (cache_size >= MAX_CACHE) {
                        /* Buffer overflow – should never happen for intended limits. */
                        return;
                    }
                    cache[cache_size++] = (seq_t)( (seq_t)4 * (seq_t)k );
                    break;          /* move to next term */
                }
            }
        }
    }
}

/* External linkage function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0 but harmless */
    ensure_sequence(n);
    return cache[n - 1];
}
