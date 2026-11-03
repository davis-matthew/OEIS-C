/*
 * A019343  Primes with primitive root 15.
 * https://oeis.org/A019343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 854 (2026-11-03)
 * Helpers:   factorize, is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A019343 Primes for which 15 is a primitive root.
 *  a(n) = the n-th prime p (n≥1) with ord_p(15)=p-1.
 *  The function builds a memoised list of such primes so that
 *  arbitrary call order is supported. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "../utils/factorize.h"
#include "../utils/powmod.h"
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

seq_t a(idx_t n)
{
    if (n == 0)        /* outside the offset – behaviour unspecified */
        return 0;

    /* Memoisation of the sequence */
    static seq_t *table  = NULL;   /* array of found primes               */
    static size_t  cap    = 0;     /* current allocation capacity (in elems) */
    static size_t  count  = 0;     /* number of primes stored             */
    static uint64_t next  = 2;     /* next integer to test                */

    /* Ensure the table can hold at least n primes */
    if (count < n) {
        if (cap < n) {
            size_t new_cap = (cap == 0) ? (n * 2) : (cap * 2);
            seq_t *tmp = mem_resize(seq_t, table, new_cap);
            if (!tmp) return 0;        /* allocation failure – give up */
            table = tmp;
            cap   = new_cap;
        }

        /* Generate primes until we have n of them */
        while (count < n) {
            /* Skip non‑primes */
            if (!is_prime_big((seq_t)next)) {
                ++next;
                continue;
            }

            /* Skip primes dividing 15 (p=3 or 5) */
            if (next == 3 || next == 5) {
                ++next;
                continue;
            }

            /* Determine whether 15 is a primitive root modulo next.
             *  This is true iff for every prime factor q of (next-1),
             *  15^((next-1)/q) mod next ≠ 1.                */
            uint64_t prime_factors[64];
            int     powers[64];
            size_t  pf_cnt = factorize(next - 1, prime_factors, powers);

            bool primitive = true;
            for (size_t i = 0; i < pf_cnt && primitive; ++i) {
                uint64_t q   = prime_factors[i];
                uint64_t exp = (next - 1) / q;
                if (powmod((seq_t)15, exp, (seq_t)next) == (seq_t)1)
                    primitive = false;
            }

            if (primitive)
                table[count++] = (seq_t)next;

            ++next;
        }
    }

    return table[n - 1];
}
