/*
 * A069848  Number of distinct primes obtained by inserting a 7 at all
 *          possible places in n. This includes prefixing as well as
 *          suffixing.
 * https://oeis.org/A069848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of distinct primes obtained by inserting the digit
 *            '7' at every possible position (including before the first digit
 *            and after the last digit) in the decimal representation of n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

/* External types provided by the build system */
extern uint64_t *_prime_cache(void);   /* dummy to silence unused warning */

/* Compute the sequence value */
seq_t a(idx_t n)
{
    /* Convert n to a decimal string (n fits in idx_t: all operations use 64-bit) */
    char s[32];
    int len = snprintf(s, sizeof(s), "%llu", (unsigned long long)n);
    if (len <= 0)
        return (seq_t)0;  /* should not occur */

    /* Store primes found to avoid counting duplicates.
       The maximum possible distinct primes is len+1 (positions 0..len). */
    uint64_t seen[20];   /* guaranteed large enough for n up to typical limits */
    int seen_cnt = 0;
    uint8_t count = 0;

    /* Try all insertion positions: 0 (prefix) to len (suffix) */
    for (int pos = 0; pos <= len; ++pos) {
        /* Build the new number string with '7' inserted at position pos */
        char t[32];
        int k = 0;

        for (int j = 0; j < pos; ++j)
            t[k++] = s[j];
        t[k++] = '7';
        for (int j = pos; j < len; ++j)
            t[k++] = s[j];
        t[k] = '\0';

        /* Convert to integer */
        uint64_t val = strtoull(t, NULL, 10);

        /* Skip non-primes */
        if (!isPrime(val))
            continue;

        /* Skip duplicates */
        bool dup = false;
        for (int i = 0; i < seen_cnt; ++i) {
            if (seen[i] == val) {
                dup = true;
                break;
            }
        }
        if (!dup) {
            seen[seen_cnt++] = val;
            ++count;
        }
    }

    return (seq_t)count;
}
