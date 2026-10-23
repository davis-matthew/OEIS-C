/*
 * A141917  Primes congruent to 13 mod 23.
 * https://oeis.org/A141917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 13 modulo 23 (offset 1)
 * Range:     u8 n <= 4, u16 n <= 233, u32 n <= 265, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime_big.h"

static seq_t *ap = NULL;          /* table of found primes           */
static size_t ap_len = 0;         /* number of primes stored         */
static size_t ap_cap = 0;         /* allocated capacity              */
static seq_t last_checked = (seq_t)12;   /* last candidate examined      */

static void ensure_primes(idx_t n)
{
    size_t target = (size_t)n;
    if (ap_len >= target) return;

    seq_t cand = last_checked + (seq_t)1;
    while (ap_len < target) {
        if ((cand % (seq_t)23) == (seq_t)13) {
            if (is_prime_big(cand)) {
                if (ap_len == ap_cap) {
                    size_t new_cap = ap_cap ? (ap_cap << 1) : 8;
                    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
                    if (ap) memcpy(new_arr, ap, ap_len * sizeof(seq_t));
                    free(ap);
                    ap = new_arr;
                    ap_cap = new_cap;
                }
                ap[ap_len++] = cand;
            }
        }
        cand = cand + (seq_t)1;
    }
    last_checked = cand - (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined; safety value */
    ensure_primes(n);
    return ap[(size_t)n - 1];
}
