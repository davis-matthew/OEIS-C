/*
 * A040098  Primes p such that x^4 = 2 has a solution mod p.
 * https://oeis.org/A040098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040098: primes p such that x^4 = 2 has a solution modulo p.
   A prime p ≠ 2 satisfies this iff p ≡ 7 (mod 8) or p ≡ 1 (mod 8) and
   2^((p−1)/4) ≡ 1 (mod p).  The function returns a(n) with the offset
   1 (i.e., a(1)=2). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq.h"

/* Cached sequence values */
static seq_t *seq      = NULL;   /* array of accepted primes */
static size_t seq_cap  = 0;      /* allocated capacity */
static size_t seq_len  = 0;      /* number of stored values */

/* The last integer that was examined when building the sequence. */
static uint64_t last_checked = 1;

/* Ensure that seq has at least n elements of storage. */
static void ensure_capacity(size_t n)
{
    if (n <= seq_cap)
        return;
    /* grow_seq expands the array to at least n elements */
    grow_seq(&seq, &seq_cap, n);
}

/* Test whether a prime p satisfies the biquadratic‑residue condition. */
static bool good_prime(uint64_t p)
{
    if (p == 2)
        return true;

    uint64_t r8 = p & 7ULL;        /* p % 8 */
    if (r8 == 7)
        return true;
    if (r8 == 1) {
        /* Compute 2^((p-1)/4) mod p. */
        seq_t base = 2;
        seq_t mod  = (seq_t)p;
        seq_t res  = mod_pow(base, (p - 1) / 4, mod);
        return res == 1;
    }
    return false;
}

/* Return the nth term (offset 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined position – return 0 as sentinel */

    /* Compute more terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        /* Find the next prime candidate. */
        do {
            ++last_checked;
        } while (!is_prime(last_checked));

        /* If it satisfies the condition, store it. */
        if (good_prime(last_checked)) {
            /* Ensure enough storage first. */
            ensure_capacity(seq_len + 1);
            seq[seq_len++] = (seq_t)last_checked;
        }
    }

    return seq[n - 1];
}
