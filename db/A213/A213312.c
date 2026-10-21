/*
 * A213312  Numbers with exactly 5 nonprime substrings (substrings with
 *          leading zeros are considered to be nonprime).
 * https://oeis.org/A213312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   a(n) = the n-th integer (in increasing order) that has exactly
 *            five nonprime decimal substrings (substrings with leading zeros
 *            are counted as nonprime). Computed by a brute‑force search
 * Range:     u8 n <= 48, u16 n <= 1245, u32 n <= 1330, u64 n <= 1330,
 *            u128 n <= 1330, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Forward declaration of helper.  seq_t and idx_t are provided by the build. */
static void build_sequence(void);

/* Static state for the sequence (memoization). */
static seq_t *seq_values = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;

/* Prime flag table for numbers up to 1,000,000.  Allocated once. */
static unsigned char *prime_flag = NULL;
static const size_t PRIME_LIMIT = 1000000;

/* Helper: builds prime_flag using a simple sieve. */
static void build_sieve(void)
{
    if (prime_flag != NULL)
        return;
    prime_flag = (unsigned char *)malloc((PRIME_LIMIT + 1) * sizeof(unsigned char));
    if (!prime_flag)
        return; /* abort silently; memory exhausted */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        prime_flag[i] = 1;
    prime_flag[0] = prime_flag[1] = 0;
    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (prime_flag[p]) {
            for (size_t m = p * p; m <= PRIME_LIMIT; m += p)
                prime_flag[m] = 0;
        }
    }
}

/* Helper: appends val to seq_values, growing if necessary. */
static void append_seq_value(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_values, new_cap * sizeof(seq_t));
        if (!new_arr) {
            /* abort silently; memory exhausted */
            return;
        }
        seq_values = new_arr;
        seq_cap = new_cap;
    }
    seq_values[seq_len++] = val;
}

/* Main function to compute the sequence. */
static void build_sequence(void)
{
    if (seq_len > 0) /* already built */
        return;

    build_sieve();

    char str[20];           /* buffer to hold decimal representation */
    unsigned int digits[20];/* store digit values for quick access */

    for (idx_t number = 10; number <= (idx_t)PRIME_LIMIT; ++number) {
        /* Convert number to string */
        int len = 0;
        idx_t tmp = number;
        while (tmp > 0) {
            digits[len] = tmp % 10;
            tmp /= 10;
            ++len;
        }
        if (len < 3) /* minimum 3 digits needed to get 5 nonprime substrings */
            continue;

        /* Reverse digits to normal order */
        for (int i = 0; i < len / 2; ++i) {
            unsigned int swap = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = swap;
        }

        unsigned int nonprime_count = 0;

        /* Enumerate all substrings */
        for (int i = 0; i < len; ++i) {
            if (digits[i] == 0) {
                /* Any substring starting with 0 is nonprime */
                for (int j = i; j < len; ++j)
                    ++nonprime_count;
            } else {
                /* Construct numeric value incrementally */
                unsigned long long val = 0;
                for (int j = i; j < len; ++j) {
                    val = val * 10 + digits[j];
                    if (val > PRIME_LIMIT) {
                        /* Substring exceeds sieve; treat as nonprime */
                        ++nonprime_count;
                    } else if (!prime_flag[(size_t)val]) {
                        ++nonprime_count;
                    }
                }
            }
        }

        if (nonprime_count == 5) {
            append_seq_value((seq_t)number);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* Undefined for n==0; return 0 */

    /* Build the sequence on first use. */
    build_sequence();

    if (n > seq_len)
        return 0; /* n beyond known terms; undefined. */

    return seq_values[n - 1];
}
