/*
 * A191080  Primes p that have Kronecker symbol (p|78) = -1.
 * https://oeis.org/A191080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime p such that the Kronecker symbol (p|78) =
 *            -1 ((p|2)*(p|3)*(p|13) = -1). The function searches successive
 *            odd numbers, tests for primality and the symbol, keeps the
 *            qualifying primes in a static array, and returns the requested
 *            index
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 938 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

seq_t a(idx_t n)
{
    /* static data for memoisation */
    static seq_t *primes = NULL;
    static size_t primes_cnt = 0;
    static seq_t candidate = 5;           /* first odd prime > 3 */
    static const size_t INIT_CAP = 50000; /* enough for n up to ~100000 */

    /* initialise on first call */
    if (!primes) {
        primes = seq_alloc_array(INIT_CAP);
        if (!primes) return 0;          /* allocation failed */
    }

    /* generate terms until we have at least n */
    while (primes_cnt < (size_t)n) {
        /* primality test */
        if (is_prime_big(candidate)) {
            /* skip primes dividing 78 */
            if (candidate != 3 && candidate != 13) {
                /* (candidate|2) : 1 if candidate ≡ ±1 (mod 8) else -1 */
                int r8 = (int)(candidate % 8);
                int sign2 = (r8 == 1 || r8 == 7) ? 1 : -1;

                /* (candidate|3) : 1 if candidate ≡ 1 (mod 3) else -1 */
                int r3 = (int)(candidate % 3);
                int sign3 = (r3 == 1) ? 1 : -1;

                /* (candidate|13) : 1 if residue is in {1,3,4,9,10,12} else -1 */
                int r13 = (int)(candidate % 13);
                bool is_res13 =
                    r13 == 1 || r13 == 3 || r13 == 4 ||
                    r13 == 9 || r13 ==10 || r13 ==12;
                int sign13 = is_res13 ? 1 : -1;

                if (sign2 * sign3 * sign13 == -1) {
                    primes[primes_cnt++] = candidate;
                }
            }
        }
        candidate += 2; /* only odd candidates after 3 */
    }

    return primes[n - 1];
}
