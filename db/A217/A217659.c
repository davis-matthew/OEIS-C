/*
 * A217659  Larger of two consecutive primes which both equal 1 (mod 3).
 * https://oeis.org/A217659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A217659: larger of two consecutive primes both ≡1 (mod 3) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"

/*
 * Computes a(n) – the n‑th larger prime in a consecutive prime pair
 * where both primes are congruent to 1 modulo 3.
 *
 * The function remembers already computed primes and sequence values
 * between calls, so that any ordering of calls works.
 */
seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1 */
        return (seq_t)0;

    /*--- Persistent state ---*/
    static uint64_t *prime_arr   = NULL;
    static size_t    prime_cap   = 0;
    static size_t    prime_cnt   = 0;
    static uint64_t  candidate   = 3;

    static seq_t  *a_arr     = NULL;
    static size_t  a_cap     = 0;
    static size_t  a_cnt     = 0;

    static size_t  scan_idx  = 0;   /* index into prime_arr being scanned */

    /*--- Generate sequence values until we have at least n terms ---*/
    while (a_cnt < (size_t)n) {
        /* Ensure we have at least two primes beyond the current scan idx */
        while (prime_cnt <= scan_idx + 1) {
            /* First prime is 2 */
            if (prime_cnt == 0) {
                if (prime_cap == 0) {
                    prime_cap = 16;
                    prime_arr = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
                }
                prime_arr[prime_cnt++] = 2;
                candidate = 3;
                continue;
            }

            /* Test candidate for primality by trial division */
            bool is_prime = true;
            /* candidate is always odd after the first prime */
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_arr[i];
                if (p > candidate / p)  /* p*p > candidate */
                    break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                /* Grow prime array if needed */
                if (prime_cnt == prime_cap) {
                    size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
                    uint64_t *tmp = (uint64_t *)realloc(prime_arr,
                                                        new_cap * sizeof(uint64_t));
                    if (!tmp) exit(1);
                    prime_arr = tmp;
                    prime_cap = new_cap;
                }
                prime_arr[prime_cnt++] = candidate;
            }
            candidate += 2;   /* next odd candidate */
        }

        /* Examine the current consecutive pair */
        uint64_t p1 = prime_arr[scan_idx];
        uint64_t p2 = prime_arr[scan_idx + 1];
        if ((p1 % 3 == 1) && (p2 % 3 == 1)) {
            /* Grow the answer array if needed */
            if (a_cnt == a_cap) {
                size_t new_cap = a_cap == 0 ? 16 : a_cap * 2;
                seq_t *tmp = seq_realloc(a_arr, a_cap, new_cap);
                if (!tmp) exit(1);
                a_arr = tmp;
                a_cap = new_cap;
            }
            a_arr[a_cnt++] = (seq_t)p2;
        }

        /* Move to the next overlapping pair */
        ++scan_idx;
    }

    return a_arr[n - 1];
}
