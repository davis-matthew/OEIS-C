/*
 * A116865  Characteristic array for partitions with only prime parts.
 * https://oeis.org/A116865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = 1 if the n-th partition in Abramowitz‑Stegun order has
 *            only prime parts, else 0. The sequence is the flattened array of
 *            all such characteristic values
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

/* Recursively generate partitions of 'sum' into 'parts' parts using part sizes
   starting from 'start'.  Partitions are generated in Abramowitz‑Stegun order
   (conjugate lexicographic).  'all_prime' tracks whether all parts yielded so
   far are prime.  When the target index 'target' is reached the result is
   stored in '*result' and true is returned to terminate the search. */
static bool gen_parts(int sum, int parts, int start, bool all_prime,
                      idx_t *counter, idx_t target, seq_t *result)
{
    if (*counter >= target)
        return true;          /* already passed the target */

    if (parts == 0) {
        if (sum == 0) {        /* finished a partition */
            (*counter)++;
            if (*counter == target) {
                *result = all_prime ? (seq_t)1 : (seq_t)0;
                return true;
            }
        }
        return false;
    }
    if (sum < parts)            /* not enough sum left for remaining parts */
        return false;
    if (start > sum)            /* smallest allowed part exceeds remaining sum */
        return false;

    int maxC = sum / start;
    if (maxC > parts)
        maxC = parts;

    for (int c = maxC; c >= 0; --c) {          /* try all counts for current part */
        bool newPrime = all_prime;
        if (c > 0)                            /* part actually used */
            newPrime = newPrime && is_prime_small((seq_t)start);

        int newSum = sum - c * start;
        int newParts = parts - c;
        if (newSum < 0 || newSum < newParts)
            continue;                         /* impossible to finish */

        if (newParts == 0) {                  /* final part to be filled */
            if (newSum == 0) {
                (*counter)++;
                if (*counter == target) {
                    *result = newPrime ? (seq_t)1 : (seq_t)0;
                    return true;
                }
            }
            continue;
        }

        if (gen_parts(newSum, newParts, start + 1, newPrime,
                      counter, target, result))
            return true;
    }
    return false;
}

/* Main sequence function.  Enumerates all partitions of integers in order,
   stopping when the target index is reached. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1, but guard anyway */
        return (seq_t)0;

    idx_t counter = 0;          /* global partition index (1‑based) */
    seq_t result  = 0;          /* value to return once target is hit */

    for (int m = 1; ; ++m) {    /* iterate over sums (i.e., numbers) */
        /* for current sum m, enumerate all partitions of any length */
        for (int len = 1; len <= m; ++len) {
            if (gen_parts(m, len, 1, true, &counter, n, &result))
                return result;
        }
    }
}
