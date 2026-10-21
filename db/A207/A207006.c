/*
 * A207006  Numbers n such that omega(n) = omega(n + omega(n)) where omega(n)
 *          is the number of distinct primes dividing n.
 * https://oeis.org/A207006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that omega(n) = omega(n + omega(n))
   where omega(n) is the number of distinct prime divisors of n.
   The function builds the sequence on demand, expanding its internal
   storage.  It is O(number_of_checked_numbers), fast enough for n <= 10000.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* helper from the utils library */
#include "../utils/count_distinct_primes.h"

/* external types supplied by the build system */
extern seq_t a(idx_t n);

/* --- internal state ------------------------------------------------------ */
static seq_t *seq_arr   = NULL;   /* array of found terms */
static size_t seq_len   = 0;      /* number of terms stored so far   */
static size_t seq_cap   = 0;      /* capacity of seq_arr              */
static uint64_t cur_num = 1;      /* next integer to be tested       */

/* --- helper functions ----------------------------------------------------- */
static void ensure_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    seq_arr = tmp;
    seq_cap = new_cap;
}

static void generate_terms(size_t target)
{
    while (seq_len < target) {
        size_t omega1 = count_distinct_primes(cur_num);
        size_t omega2 = count_distinct_primes(cur_num + omega1);
        if (omega1 == omega2) {
            ensure_capacity(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)cur_num;
        }
        cur_num++;
    }
}

/* --- sequence entry point ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) abort();       /* sequence starts at index 1 */
    generate_terms((size_t)n);
    return seq_arr[n - 1];
}
