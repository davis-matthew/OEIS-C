/*
 * A074381  (p-1)/2 mod 2, where p is the n-th prime for which p+2 is also
 *          prime; i.e., a(n)=0 if p==1 (mod 4), a(n)=1 if p==3 (mod 4).
 * https://oeis.org/A074381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if the lower prime of the n‑th twin‑prime pair is ≡ 3
 *            (mod 4), otherwise 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   grow_seq, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility headers */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

/* Static state for generating twin primes */
static idx_t          last_checked = 3;   /* next odd candidate to test */
static seq_t *        twin_primes  = NULL;
static size_t        twin_count   = 0;
static size_t        twin_cap     = 0;

/* Ensure that we have computed at least n twin primes */
static void ensure_twin_prime(idx_t n)
{
    while (twin_count < n) {
        /* test whether last_checked and last_checked+2 are both prime */
        if (is_prime(last_checked) && is_prime(last_checked + 2)) {
            /* grow the array if necessary */
            if (twin_count == twin_cap) {
                grow_seq(&twin_primes, &twin_cap, twin_count + 1);
            }
            twin_primes[twin_count++] = last_checked;
        }
        /* move to next odd candidate */
        last_checked += 2;
    }
}

/* Sequence function: a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of bounds – not used */
    ensure_twin_prime(n);                /* make sure we have the n‑th twin prime */

    seq_t p = twin_primes[n - 1];        /* lower member of the pair */
    return (p & 3) == 3 ? 1 : 0;         /* parity of (p‑1)/2 */
}
