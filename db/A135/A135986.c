/*
 * A135986  a(1)=1; a(n) = a(n-1) + d(n) if d(n) > d(n-1); a(n) = a(n-1) -
 *          d(n) if d(n) < d(n-1); a(n) = a(n-1) if d(n) = d(n-1), where d =
 *          A000005.
 * https://oeis.org/A135986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A135986
   a(1) = 1.
   For n≥2 let d_k = number of divisors of k (A000005). 
   If d_n > d_{n-1} then a(n) = a(n-1) + d_n;
   if d_n < d_{n-1} then a(n) = a(n-1) - d_n;
   if d_n = d_{n-1} then a(n) = a(n-1).
*/

#include <stdlib.h>
#include <stdint.h>

static seq_t divisor_count(idx_t n)
{
    seq_t cnt = 1;
    idx_t temp = n;

    if (temp % 2 == 0) {
        idx_t exp = 0;
        while ((temp % 2) == 0) {
            temp /= 2;
            exp++;
        }
        cnt *= (exp + 1);
    }
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            cnt *= (exp + 1);
        }
    }
    if (temp > 1) cnt *= 2;   /* remaining prime factor */
    return cnt;
}

seq_t a(idx_t n)
{
    /* Allow out‑of‑range calls (should not happen for OEIS usage) */
    if (n < 1) return 0;

    static seq_t *a_cache = NULL;
    static idx_t max_cached = 0;
    static seq_t last_d = 0;   /* d(max_cached) when max_cached > 0 */

    /* Return cached value if already computed */
    if (n <= max_cached) return a_cache[n];

    /* Allocate or grow the cache */
    idx_t new_cap = n + 1;   /* indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) return 0;      /* out of memory – in real use handle error */
    a_cache = tmp;

    /* Initialize base case if this is the first call */
    if (max_cached == 0) {
        a_cache[1] = 1;
        max_cached = 1;
        last_d = divisor_count(1);   /* 1 has one divisor */
    }

    /* Extend the sequence from max_cached+1 up to n */
    seq_t prev_a = a_cache[max_cached];
    seq_t prev_d = last_d;

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t d = divisor_count(i);
        seq_t new_a;
        if (d > prev_d) new_a = prev_a + d;
        else if (d < prev_d) new_a = prev_a - d;
        else new_a = prev_a;

        a_cache[i] = new_a;
        prev_a = new_a;
        prev_d = d;
    }

    /* Update state */
    max_cached = n;
    last_d = prev_d;
    return a_cache[n];
}
