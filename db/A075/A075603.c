/*
 * A075603  Number of relatively prime numbers using all digits of n.
 * https://oeis.org/A075603
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 115, u16 n <= 115, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the number of permutations of all decimal
   digits of n that are coprime to n.  The permutations are taken
   as integers, allowing leading zeros which produce shorter numbers. */

/* Standard headers only */
#include <stdbool.h>

/* Forward declaration of the sequence type */
#ifdef NULL
#undef NULL
#endif

/* GCD for two uint64_t values, result returned as seq_t.  */
static inline seq_t gcd_uint64(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return (seq_t)a;
}

/* Recursive helper that generates all distinct permutations of the
   multiset described by cnt[10] (digit counts).  cur is the number
   built so far.  len is the total number of digits.  nval is the
   original number.  ans accumulates the count.  */
static void permute_unique(int pos, idx_t cur,
                           uint8_t *cnt, int len,
                           idx_t nval, seq_t *ans)
{
    if (pos == len) {
        if (gcd_uint64(nval, cur) == (idx_t)1)
            (*ans)++;
        return;
    }
    for (int d = 0; d <= 9; ++d) {
        if (cnt[d]) {
            cnt[d]--;
            permute_unique(pos + 1, cur * 10 + d,
                           cnt, len, nval, ans);
            cnt[d]++;
        }
    }
}

seq_t a(idx_t n) {
    /* Extract decimal digits of n.  */
    uint8_t digits[32];
    int len = 0;
    if (n == 0) {
        digits[len++] = 0;
    } else {
        idx_t tmp = n;
        while (tmp > 0) {
            digits[len++] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Count occurrences of each digit.  */
    uint8_t cnt[10] = {0};
    for (int i = 0; i < len; ++i)
        cnt[digits[i]]++;

    /* Count coprime permutations.  */
    seq_t ans = 0;
    permute_unique(0, 0, cnt, len, n, &ans);
    return ans;
}
