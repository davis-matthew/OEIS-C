/*
 * A165666  Primes p such that (p^2-2)/7 is not prime.
 * https://oeis.org/A165666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 180, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A165666: primes p such that (p^2-2)/7 is not prime */

/* The function a(n) returns the n‑th prime p satisfying the condition.
   It builds the sequence incrementally and stores the terms in a static
   array.  The array capacity is set large enough to cover the required
   range (≈10000 terms).  No dynamic reallocation is performed.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

static /* maximum number of terms that can be stored – generous for 10 k */
       const size_t MAX_CAP = 3000000;
static seq_t *seq_arr      = NULL; /* array of sequence values          */
static size_t seq_len      = 0;     /* current number of stored terms */
static idx_t  last_checked = 1;     /* last integer examined           */
static bool   initialized  = false;

/* Initialise the static array once. */
static void ensure_initialised(void)
{
    if (initialized) return;
    seq_arr = (seq_t*)malloc(MAX_CAP * sizeof(seq_t));
    initialized = true;
}

/* Generate sequence terms until we have at least n terms. */
static void generate_until(idx_t n)
{
    ensure_initialised();
    while (seq_len < (size_t)n) {
        ++last_checked;                     /* next candidate integer   */
        if (!is_prime(last_checked)) continue;
        seq_t cc   = (seq_t)last_checked;
        seq_t diff = cc * cc - (seq_t)2;      /* p^2 - 2                 */
        if (diff % (seq_t)7 != (seq_t)0) {
            seq_arr[seq_len++] = cc;
            continue;
        }
        seq_t q = diff / (seq_t)7;           /* (p^2-2)/7               */
        if (!is_prime_big(q)) {
            seq_arr[seq_len++] = cc;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1 – ignore n=0    */
    generate_until(n);
    return seq_arr[(size_t)n - 1];
}
