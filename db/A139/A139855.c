/*
 * A139855  Primes of the form 4x^2+4xy+31y^2.
 * https://oeis.org/A139855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime of the form 4x^2+4xy+31y^2 (OEIS A139855)
 * Range:     u8 n <= 4, u16 n <= 412, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 58 terms through n = 8768 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Returns the nth element of the sequence (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but harmless */

    /* static state to remember already found terms across calls */
    static seq_t *list   = NULL;   /* array holding the primes found so far */
    static size_t cap    = 0;      /* allocated capacity of the array       */
    static idx_t  count  = 0;      /* number of primes stored               */

    /* next candidates of the two residue classes */
    static seq_t next31 = (seq_t)31;   /* numbers ≡ 31 (mod 120) */
    static seq_t next79 = (seq_t)79;   /* numbers ≡ 79 (mod 120) */

    /* ensure we have at least n terms precomputed */
    while (count < n)
    {
        /* pick the smaller candidate from the two progressions */
        seq_t cand = (next31 <= next79) ? next31 : next79;

        /* test primality */
        if (is_prime_big(cand))
        {
            /* grow storage if needed */
            if ((size_t)count >= cap)
            {
                size_t newcap = (cap == 0) ? 16 : cap << 1;
                seq_t *tmp = mem_resize(seq_t, list, newcap);
                if (tmp == NULL) /* allocation failed – abort gracefully */
                    return 0;
                list = tmp;
                cap  = newcap;
            }
            list[count++] = cand;
        }

        /* advance the progression that supplied the candidate */
        if (cand == next31)
            next31 += (seq_t)120;
        else
            next79 += (seq_t)120;
    }

    return list[n-1];
}
