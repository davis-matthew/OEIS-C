/*
 * A165637  Primes p such that (p^2-3)/2 is not a prime number.
 * https://oeis.org/A165637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime p such that (p^2-3)/2 is not a prime number
 * Range:     u8 n <= 2, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 965 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    static seq_t *special = NULL;
    static size_t special_cap = 0;
    static size_t special_cnt = 0;
    static uint64_t next_candidate = 2;

    if (n == 0) return (seq_t)0;   /* Safety for n=0, though not used */

    while (special_cnt < (size_t)n) {
        for (;;) {
            seq_t cand_seq = (seq_t)next_candidate;
            bool cand_is_prime = is_prime_big(cand_seq);
            uint64_t cand_val = next_candidate;
            ++next_candidate;           /* advance for next iteration */

            if (!cand_is_prime) continue;

            if (cand_val == 2) {        /* include 2 explicitly */
                if (special_cnt == special_cap) {
                    size_t new_cap = special_cap ? special_cap * 2 : 8;
                    special = mem_resize(seq_t, special, new_cap);
                    special_cap = new_cap;
                }
                special[special_cnt++] = (seq_t)2;
                break;
            } else {
                /* compute (p^2-3)/2 */
                seq_t expr = ((cand_seq * cand_seq - (seq_t)3) / (seq_t)2);
                if (!is_prime_big(expr)) {
                    if (special_cnt == special_cap) {
                        size_t new_cap = special_cap ? special_cap * 2 : 8;
                        special = mem_resize(seq_t, special, new_cap);
                        special_cap = new_cap;
                    }
                    special[special_cnt++] = cand_seq;
                    break;
                }
            }
        }
    }
    return special[(size_t)n - 1];
}
