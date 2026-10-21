/*
 * A230507  Number of ways to write n = a + b + c with a <= b <= c, where a,
 *          b, c are among those numbers m (terms of A230506) with 2*m + 1 and
 *          2*m^3 + 1 both prime.
 * https://oeis.org/A230507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4449, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ways to write n = a+b+c with a <= b <= c,
   where a, b, c belong to the set S = { m | 2*m+1 and 2*m^3+1 are prime }.
   The function builds S up to the requested n and counts triples
   by enumerating pairs (a,b) and checking if the required c exists.
   Time complexity is O(|S|^2 log|S|) per call, which is fast for n up to
   at least 10 000. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime.h"

static idx_t *valid_m = NULL;   /* sorted list of m in S */
static size_t vlen   = 0;       /* number of elements in valid_m   */
static size_t vcap   = 0;       /* capacity of valid_m             */
static idx_t last_m_processed = 0; /* highest m inspected so far  */

/* Extend the list valid_m to include all m <= needed that satisfy
   the primality conditions.  */
static void extend_to(idx_t needed)
{
    if (needed <= last_m_processed)
        return;

    for (idx_t m = last_m_processed + 1; m <= needed; ++m) {
        /* Check 2*m+1 prime first (quick filter). */
        if (!is_prime(2 * m + 1))
            continue;

        /* Compute 2*m^3+1 safely using 128‑bit intermediate.
           If the value overflows 64 bits we skip it, as we
           cannot test its primality with the available helper. */
        __int128 t = (__int128)2 * m * m * m + 1;
        if (t > ( (__int128)UINT64_MAX ))
            continue;

        if (is_prime((idx_t)t)) {
            /* Append m to the list, resizing if necessary. */
            if (vlen == vcap) {
                vcap = (vcap == 0) ? 4 : vcap * 2;
                idx_t *tmp = (idx_t *)realloc(valid_m, vcap * sizeof(idx_t));
                valid_m = tmp;
            }
            valid_m[vlen++] = m;
        }
    }
    last_m_processed = needed;
}

/* Count triples a <= b <= c from S such that a+b+c = n. */
static seq_t count_triples(idx_t n)
{
    seq_t count = 0;

    /* For each unordered pair (a,b) with a <= b, try to find c. */
    for (size_t i = 0; i < vlen; ++i) {
        idx_t a_val = valid_m[i];
        for (size_t j = i; j < vlen; ++j) {
            idx_t b_val = valid_m[j];
            idx_t sum_ab = a_val + b_val;
            if (sum_ab > n)           /* further j will only increase sum_ab */
                break;

            idx_t remaining = n - sum_ab;
            if (remaining < b_val)    /* c must be >= b_val as nondecreasing */
                continue;

            /* Binary search for remaining in valid_m[j .. vlen-1] */
            size_t lo = j, hi = vlen;
            while (lo < hi) {
                size_t mid = lo + (hi - lo) / 2;
                if (valid_m[mid] < remaining)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            if (lo < vlen && valid_m[lo] == remaining)
                count += 1;
        }
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0ULL;              /* offset starts at 1 */

    extend_to(n);                 /* ensure all m <= n are in valid_m */
    return count_triples(n);
}
