/*
 * A050986  Number of n-digit right-truncatable primes.
 * https://oeis.org/A050986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function counts n‑digit right‑truncatable primes by building all such
   primes recursively.  
   The recursion starts from the single‑digit primes 2,3,5,7 and appends
   the digits {1,3,7,9} whenever the new number is prime.
   The counts for each length (1..8) are stored in a static array.
   Since there are no right‑truncatable primes of length ≥9, the function
   returns 0 for any n ≥ 9.
*/
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t counts[9] = {0};   /* indices 1..8 used */
static bool initialized = false;

/* digits that can be appended to form a prime (ending primes ≡ 1,3,7,9 mod 10) */
static const int append_digits[4] = {1, 3, 7, 9};

/* Recursive DFS: cur is a prime of length len.
   For each possible appended digit, if the new number is prime,
   we increment the count for length len+1 and recurse further. */
static void dfs(uint64_t cur, int len)
{
    if (len >= 8) return;          /* maximum length of right‑truncatable primes */
    for (int i = 0; i < 4; ++i) {
        uint64_t cand = cur * 10 + append_digits[i];
        if (is_prime_small(cand)) {
            counts[len + 1]++;     /* cand has length len+1 */
            dfs(cand, len + 1);
        }
    }
}

/* Initialize the counts array once. */
static void init_counts(void)
{
    if (initialized) return;
    /* start with single‑digit primes */
    const int base_primes[4] = {2, 3, 5, 7};
    for (int i = 0; i < 4; ++i) {
        counts[1]++;                /* each one‑digit prime */
        dfs(base_primes[i], 1);
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for non‑positive index */
    if (n > 8) return 0;           /* no primes of length ≥9 */
    init_counts();
    return counts[n];
}
