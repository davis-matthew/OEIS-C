/*
 * A069842  Number of distinct primes obtained by inserting a 1 at all
 *          possible places in n. This includes prefixing and suffixing.
 * https://oeis.org/A069842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of distinct primes obtained by inserting the digit
 * 1 into every possible position (including before the first and after the
 * last digit) of the decimal representation of n.
 *
 * This implementation builds the inserted numbers as strings, converts
 * them to integers, tests primality via the provided isPrime(idx_t) helper,
 * and counts each prime only once.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1; return 0 for n==0 */
        return 0;

    /* Convert n to its decimal string representation */
    char str[32];
    int len = 0;
    idx_t tmp = n;
    /* Handle n==0 separately (though n>0 here) */
    if (tmp == 0) {
        str[len++] = '0';
    } else {
        /* Extract digits in reverse order */
        char rev[32];
        int rev_len = 0;
        while (tmp > 0) {
            rev[rev_len++] = '0' + (tmp % 10);
            tmp /= 10;
        }
        /* Reverse to get the correct order */
        for (int i = 0; i < rev_len; ++i)
            str[i] = rev[rev_len - 1 - i];
        len = rev_len;
    }
    str[len] = '\0';

    /* Buffer for each inserted number */
    char insert_buf[64];

    /* Store primes seen so far (upper bound: len+1 positions) */
    idx_t seen_primes[32];
    int seen_cnt = 0;

    seq_t count = 0;

    /* Try all insertion positions, from 0 (before first digit) to len (after last) */
    for (int pos = 0; pos <= len; ++pos) {
        /* Build new string: keep characters before pos, insert '1', then remaining */
        memcpy(insert_buf, str, pos);
        insert_buf[pos] = '1';
        memcpy(insert_buf + pos + 1, str + pos, len - pos);
        insert_buf[len + 1] = '\0';

        /* Convert to integer */
        unsigned long long val = strtoull(insert_buf, NULL, 10);
        idx_t v = (idx_t)val;

        /* Test primality */
        if (!isPrime(v))
            continue;

        /* Check if this prime was already counted */
        bool already = false;
        for (int i = 0; i < seen_cnt; ++i)
            if (seen_primes[i] == v) {
                already = true;
                break;
            }

        if (!already) {
            seen_primes[seen_cnt++] = v;
            ++count;
        }
    }

    return count;
}
