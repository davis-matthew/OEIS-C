/*
 * A115350  Termination of the aliquot sequence starting at n.
 * https://oeis.org/A115350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   floor_sqrt, visited_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Termination of the aliquot sequence starting at n
   a(n) = prime if the sequence ends in prime → 1,
          perfect number if it reaches a perfect number,
          smallest element of a cycle if it becomes cyclic,
          0 if it never terminates within a reasonable bound.
*/
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/visited_index.h"

/* Compute the sum of proper divisors of n (n >= 1) */
static seq_t sum_proper_div(seq_t n)
{
    if (n == 0 || n == 1)
        return 0;
    seq_t sum = 1;                    /* 1 is a proper divisor of all n > 1 */
    seq_t temp = n;
    seq_t max = floor_sqrt(temp);     /* upper bound for trial division   */
    for (seq_t p = 2; p <= max; ++p) {
        if (temp % p == 0) {
            seq_t power = 1;          /* p^0 */
            seq_t coeff = 1;          /* 1 + p + p^2 + ... */
            while (temp % p == 0) {
                temp /= p;
                power *= p;
                coeff += power;
            }
            sum *= coeff;             /* multiply by divisor sum for this prime power */
            max = floor_sqrt(temp);   /* may shrink after reducing temp */
        }
    }
    if (temp > 1)                     /* remaining prime factor > sqrt(original n) */
        sum *= (seq_t)1 + temp;
    return sum - n;                   /* exclude n itself */
}

/* Alias for the maximum number of steps we will follow
   before declaring the sequence open‑ended. */
#define MAX_STEPS 5000

seq_t a(idx_t n)
{
    /* n is 1‑based; a(1)=1 by OEIS definition. */
    if (n == 1)
        return 1;

    /* Static array that stores the visited values for this run.
       The array is cleared at the start of each call. */
    static seq_t visited[MAX_STEPS];
    size_t visited_len = 0;

    seq_t cur = (seq_t)n;
    visited[visited_len++] = cur;

    for (size_t step = 0; step < MAX_STEPS; ++step) {
        seq_t next = sum_proper_div(cur);

        /* Terminate in a prime: next == 1 */
        if (next == 1)
            return cur;

        /* Perfect number: next == cur */
        if (next == cur)
            return cur;

        /* Cycle detection */
        int idx = visited_index(visited, visited_len, next);
        if (idx != -1) {
            /* Find minimal element in the cycle */
            seq_t min_cycle = visited[idx];
            for (size_t i = (size_t)idx; i < visited_len; ++i) {
                if (visited[i] < min_cycle)
                    min_cycle = visited[i];
            }
            return min_cycle;
        }

        if (visited_len >= MAX_STEPS)
            break;   /* safeguard against overflow */

        visited[visited_len++] = next;
        cur = next;
    }

    /* If we reach here, the sequence did not terminate within
       the allowed number of steps. Return 0 as the OEIS definition
       requires for open‑ended sequences. */
    return 0;
}
