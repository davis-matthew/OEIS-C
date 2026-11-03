/*
 * A060245  Primes p such that the sum of the first p composite numbers is
 *          itself prime.
 * https://oeis.org/A060245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060245: primes p such that the sum of the first p composite
 * numbers is itself prime.
 * The function incrementally generates composite numbers and records
 * primes that satisfy the condition. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
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

/* Return the nth term (n ≥ 1). */
seq_t a(idx_t n)
{
    /* cached list of found primes */
    static seq_t *solutions = NULL;
    static size_t sol_cap   = 0;
    static size_t sol_len   = 0;

    /* state for generating composites */
    static idx_t next_num = 4;           /* next integer to test */
    static idx_t comp_cnt = 0;           /* how many composites seen */
    static seq_t comp_sum = 0;           /* sum of those composites */

    /* generate terms until we have at least n of them */
    while (sol_len < n)
    {
        /* find the next composite number */
        while (1)
        {
            if (!is_prime(next_num))          /* composite */
            {
                ++comp_cnt;
                comp_sum += (seq_t)next_num;
                ++next_num;
                break;
            }
            ++next_num;        /* skip primes */
        }

        /* if the count of composites is prime and the sum is prime,
           record the count as the next term of the sequence */
        if (is_prime(comp_cnt) && is_prime_big(comp_sum))
        {
            if (sol_len == sol_cap)
            {
                size_t new_cap = sol_cap == 0 ? 16 : sol_cap * 2;
                seq_t *tmp = mem_resize(seq_t, solutions, new_cap);
                if (!tmp) abort();          /* allocation failed */
                solutions = tmp;
                sol_cap = new_cap;
            }
            solutions[sol_len++] = (seq_t)comp_cnt;
        }
    }

    return solutions[n - 1];
}
