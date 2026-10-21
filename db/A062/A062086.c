/*
 * A062086  Squarefree numbers with all odd digits.
 * https://oeis.org/A062086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth squarefree number whose decimal digits are all
 *            odd. The sequence starts at n=1 (a(1)=1). This implementation
 *            generates odd‑digit numbers in increasing order and tests each
 *            for square‑freeness
 * Range:     u8 n <= 43, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   init_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/init_primes.h"

/* static helper that checks whether val is squarefree using the precomputed primes */
static bool is_squarefree(const seq_t val, const int *primes, int primes_cnt)
{
    for (int i = 0; i < primes_cnt; ++i) {
        int p = primes[i];
        seq_t pseq = (seq_t)p;
        seq_t p2 = pseq * pseq;
        if (p2 > val) break;
        if (val % p2 == 0) return false;
    }
    return true;
}

/* recursive generator that stops once the nth term is found */
static void gen_len_stop(int len, int pos, seq_t cur, idx_t n,
                         idx_t *count, seq_t *result, bool *found,
                         const int *primes, int primes_cnt)
{
    if (*found) return;
    if (pos == len) {
        if (is_squarefree(cur, primes, primes_cnt)) {
            (*count)++;
            if (*count == n) {
                *result = cur;
                *found = true;
            }
        }
        return;
    }
    static const int digits[5] = {1, 3, 5, 7, 9};
    for (int d = 0; d < 5; ++d) {
        seq_t next = cur * 10 + (seq_t)digits[d];
        gen_len_stop(len, pos + 1, next, n, count, result, found,
                     primes, primes_cnt);
        if (*found) return;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset is 1; n>=1 normally */

    /* Prime list for squarefree testing */
    static bool primes_ready = false;
    static int primes[20000];     /* more than enough for LIMIT=100000 */
    static int primes_cnt = 0;

    if (!primes_ready) {
        init_primes(primes, &primes_cnt);
        primes_ready = true;
    }

    idx_t count = 0;
    seq_t result = 0;
    bool found = false;

    for (int len = 1; !found; ++len) {
        gen_len_stop(len, 0, (seq_t)0, n, &count, &result, &found,
                     primes, primes_cnt);
    }

    return result;
}
