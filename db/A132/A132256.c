/*
 * A132256  Isolated primes congruent to {1, 29} mod 30.
 * https://oeis.org/A132256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the nth isolated prime congruent to 1 or 29 (mod 30)
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

/* Advance to the next odd candidate (except 2) */
static inline idx_t next_candidate(idx_t curr)
{
    return (curr <= 2) ? 3 : curr + 2;
}

static seq_t *isolated = NULL;          /* array of isolated primes */
static size_t isolated_cnt = 0;         /* number of stored elements */
static size_t isolated_cap = 0;         /* allocated capacity */

static idx_t tested_candidate = 2;      /* next number to test */

/* Grow the isolated array if necessary */
static void ensure_isolated_capacity(void)
{
    if (isolated_cnt >= isolated_cap) {
        isolated_cap = isolated_cap ? isolated_cap * 2 : 4;
        isolated = mem_resize(seq_t, isolated, isolated_cap);
    }
}

/* Generate isolated primes until we have at least 'target' entries. */
static void add_isolated_until(idx_t target)
{
    while (isolated_cnt < target) {
        if (is_prime_big((seq_t)tested_candidate)) {
            /* Check modulus 30 constraint */
            if (tested_candidate % 30 == 1 || tested_candidate % 30 == 29) {
                bool isolated_ok = true;

                /* p-2 */
                if (tested_candidate >= 3 &&
                    is_prime_big((seq_t)(tested_candidate - 2)))
                    isolated_ok = false;
                /* p-1 */
                if (isolated_ok &&
                    is_prime_big((seq_t)(tested_candidate - 1)))
                    isolated_ok = false;
                /* p+1 */
                if (isolated_ok &&
                    is_prime_big((seq_t)(tested_candidate + 1)))
                    isolated_ok = false;
                /* p+2 */
                if (isolated_ok &&
                    is_prime_big((seq_t)(tested_candidate + 2)))
                    isolated_ok = false;

                if (isolated_ok) {
                    ensure_isolated_capacity();
                    isolated[isolated_cnt++] = (seq_t)tested_candidate;
                }
            }
        }
        tested_candidate = next_candidate(tested_candidate);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n = 0; return 0 to avoid undefined behaviour. */
        return (seq_t)0;
    }
    add_isolated_until(n);
    return isolated[n - 1];
}
