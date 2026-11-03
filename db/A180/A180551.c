/*
 * A180551  Primes that become a different prime under the mapping 5 <=> 6.
 * https://oeis.org/A180551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th prime that when all digits 5 and 6 are swapped
 *            (5 ↔ 6) yields a different prime. Computed by a simple
 *            sieve-free primality test on successive integers
 * Range:     u8 n <= 2, u16 n <= 579, u32 n <= 2080, u64 n <= 2080,
 *            u128 n <= 2080, big any n
 * Validated: standard, pass, 61 terms through n = 1784 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

static seq_t swap56(seq_t n)
{
    seq_t res = 0;
    seq_t mul = 1;
    const seq_t TEN = (seq_t)10;
    while (n > 0) {
        uint8_t d = (uint8_t)(n % TEN);
        if (d == 5) d = 6;
        else if (d == 6) d = 5;
        res = res + (seq_t)d * mul;
        mul = mul * TEN;
        n = n / TEN;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;          /* safety for indices < 1 */

    /* Persisted state across calls */
    static idx_t next = 2;          /* next integer to test for primality   */
    static seq_t *seq = NULL;       /* array of found terms                 */
    static size_t cap = 0;          /* capacity of seq array                */
    static size_t len = 0;          /* number of terms stored               */

    while (len < n) {
        seq_t cur = (seq_t)next;
        if (is_prime_big(cur)) {
            seq_t trans = swap56(cur);
            if (trans != cur && is_prime_big(trans)) {
                if (len == cap) {
                    cap = (cap == 0) ? 16 : cap * 2;
                    seq = mem_resize(seq_t, seq, cap);
                }
                seq[len++] = cur;
            }
        }
        ++next;
    }
    return seq[n - 1];
}
