/*
 * A156683  Integers that can occur as either leg in more than one primitive
 *          Pythagorean triple.
 * https://oeis.org/A156683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156683: integers that can occur as either leg in more
 * than one primitive Pythagorean triple.
 *
 * A term has at least two distinct prime factors and is not a singly‑even
 * number (i.e., it is either odd or divisible by 4).  The function
 * generates the terms by gradually testing successive integers.
 * This implementation memoizes the results so that calls with random
 * indices are efficient.  Complexity per term is O(√n) due to the
 * factor‑count helper `count_distinct_primes`.  For n up to about
 * 10000 the runtime is well below 2 s. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0) return (seq_t)0;

    /* static storage for memoised sequence */
    static seq_t *seq  = NULL;          /* array of computed terms   */
    static size_t seq_len  = 0;         /* number of stored terms    */
    static size_t seq_cap  = 0;         /* current capacity         */
    static uint64_t cur_num = 1;        /* candidate number for test*/

    /* Ensure we have at least n terms computed */
    while (seq_len < n) {
        /* step to next candidate */
        cur_num++;

        /* count distinct prime factors of cur_num */
        uint64_t primes = count_distinct_primes(cur_num);

        /* reject if not enough distinct primes */
        if (primes < 2) continue;

        /* reject singly‑even numbers (2 * odd) */
        if ((cur_num & 1ULL) == 0 && (cur_num % 4ULL) != 0) continue;

        /* accept current number */
        if (seq_len == seq_cap) {
            size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
            assert(tmp != NULL);
            seq = tmp;
            seq_cap = new_cap;
        }
        seq[seq_len++] = (seq_t)cur_num;
    }
    return seq[n - 1];
}
