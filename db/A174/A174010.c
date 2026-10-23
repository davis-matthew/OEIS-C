/*
 * A174010  Primes p of the form p = A000040(k) - A163300(k) for some k
 *          (includes duplicates).
 * https://oeis.org/A174010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = p_k – q_k, where p_k is the k‑th prime and q_k is the
 *            k‑th even nonnegative nonprime (0 for k=1, 2k otherwise). The
 *            function scans k = 1,2,…, generating values whenever p_k – q_k
 *            is a prime
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/get_prime.h"         /* uint32_t get_prime(idx_t k) */
#include "../utils/isPrimeGeneric.h"    /* bool isPrimeGeneric(seq_t n) */

static seq_t *seq_values = NULL;   /* dynamic array of sequence values   */
static size_t seq_cap    = 0;       /* current capacity of seq_values    */
static size_t seq_len    = 0;       /* number of values already stored   */
static idx_t  next_k     = 1;       /* next k to process (1‑based)       */

static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_values = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined by definition */

    /* Generate values until we have at least n of them. */
    while (seq_len < (size_t)n) {
        uint32_t pk   = get_prime(next_k);          /* k‑th prime (uint32_t) */
        uint64_t sub = (next_k == 1) ? 0ULL
                                     : 2ULL * (uint64_t)next_k;

        if ((uint64_t)pk < sub) {                   /* difference negative */
            ++next_k;
            continue;
        }

        uint64_t diff = (uint64_t)pk - sub;         /* non‑negative   */
        if (diff >= 2) {
            seq_t d = (seq_t)diff;
            if (isPrimeGeneric(d)) {
                ensure_seq_capacity(seq_len + 1);
                seq_values[seq_len++] = d;
            }
        }
        ++next_k;
    }
    return seq_values[n - 1];
}
