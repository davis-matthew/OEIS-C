/*
 * A049543  Primes p such that x^11 = 2 has a solution mod p.
 * https://oeis.org/A049543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq, free_seq, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p for which the congruence x^11 ≡ 2 (mod p) has a
   solution.  For each prime p>2 let g = gcd(11, p-1). Then the congruence
   is solvable iff 2^((p-1)/g) ≡ 1 (mod p).  Since 11 is prime,
   g is either 1 or 11; when g = 1 the condition is automatically true,
   otherwise we test 2^((p-1)/11) modulo p. *
   The code enumerates primes in increasing order, caching the found terms
   so that calls to a() are independent of order and efficient. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static void ensure_cap_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 128 : (*cap << 1);
    while (newcap < needed) newcap <<= 1;
    seq_t *new_arr = alloc_seq(newcap);
    if (*arr) {
        for (size_t i = 0; i < *cap; ++i) new_arr[i] = (*arr)[i];
        free_seq(*arr);
    }
    *arr = new_arr;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;          /* cached primes satisfying the condition */
    static size_t sz = 0, cap = 0;        /* size and capacity of the cache */

    static uint64_t cand = 3;             /* next odd number to test */

    /* initialise with the first prime (2) */
    if (sz == 0) {
        ensure_cap_seq(&primes, &cap, 1);
        primes[0] = (seq_t)2;
        sz = 1;
    }

    while (sz < (size_t)n) {
        /* --- find next prime --- */
        while (!is_prime(cand)) cand += 2;

        uint64_t p = cand;
        cand += 2;           /* prepare for next candidate */

        /* --- check 11th–power residue condition --- */
        bool include = false;
        if (p == 2) {                      /* special case: 0^11 ≡ 2 (mod 2) */
            include = true;
        } else {
            if ((p - 1) % 11ULL != 0ULL) {
                /* gcd(11,p-1) == 1  → condition automatically satisfied */
                include = true;
            } else {
                uint64_t exp = (p - 1) / 11ULL;
                seq_t mod = (seq_t)p;
                seq_t res = mod_pow((seq_t)2, exp, mod);
                include = (res == (seq_t)1);
            }
        }

        if (include) {
            ensure_cap_seq(&primes, &cap, sz + 1);
            primes[sz++] = (seq_t)p;
        }
    }

    return primes[n - 1];
}
