/*
 * A105145  Numbers n such that the string n10001 is prime.
 * https://oeis.org/A105145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = n such that the number formed by concatenating n and
 *            10001 (i.e. n*10^5 + 10001) is prime
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   isprime_big, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isprime_big.h"
#include "../utils/push_seq.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;        /* cached list of found terms */
    static idx_t seq_cap = 0;
    static idx_t seq_len = 0;
    static idx_t test_n = 1;        /* next n to test */

    while (seq_len < n) {
        seq_t val = (seq_t)test_n * (seq_t)100000 + (seq_t)10001;
        if (isprime_big(val))
            push_seq(&seq, &seq_cap, &seq_len, (seq_t)test_n);
        test_n++;
    }
    return seq[n - 1];
}
