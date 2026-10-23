/*
 * A103548  a(n) is the largest n-digit zeroless prime such that the sum of
 *          the two numbers that result from splitting a(n) between any two of
 *          its digits is a distinct prime.
 * https://oeis.org/A103548
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_prime_seq, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the largest n‑digit zeroless prime such that all split sums
   are distinct primes. Brute‑force search for n ≤ 7; returns 0 for n > 7. */
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/is_prime_seq.h"

static const int first_digits[4] = {8, 6, 4, 2};
static const int last_digits[4]  = {9, 7, 3, 1};

static bool valid_split_sums(seq_t num, idx_t n)
{
    seq_t sums[64];  /* n ≤ 7 here, 64 is ample */
    int count = 0;
    for (idx_t j = 1; j < n; ++j) {
        seq_t pow = pow10(n - j);
        seq_t left = num / pow;
        seq_t right = num % pow;
        seq_t sum = left + right;
        if (!is_prime_seq(sum))
            return false;
        for (int k = 0; k < count; ++k)
            if (sums[k] == sum)
                return false;
        sums[count++] = sum;
    }
    return true;
}

static seq_t search_rec(idx_t pos, idx_t n, seq_t cur)
{
    if (pos == n) {
        if (!is_prime_seq(cur))
            return 0;
        return valid_split_sums(cur, n) ? cur : 0;
    }
    const int *digits = (pos == n - 1) ? last_digits : first_digits;
    for (int i = 0; i < 4; ++i) {
        seq_t nxt = cur * 10 + digits[i];
        seq_t res = search_rec(pos + 1, n, nxt);
        if (res != 0)
            return res;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    if (n > 7)
        return 0;
    return search_rec(0, n, 0);
}
