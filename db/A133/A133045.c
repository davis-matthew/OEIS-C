/*
 * A133045  Successive digits of Fermat numbers A000215(n).
 * https://oeis.org/A133045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4941, u16 n <= 4941, u32 n <= 4941, u64 n <= 4941,
 *            u128 n <= 4941, big any n
 * Helpers:   bigint_to_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Successive digits of Fermat numbers: a(n) = n-th decimal digit of the
   concatenation of 2^(2^k)+1 for k ≥ 0. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../utils/bigint_to_decimal.h"

#define BASE      1000000000U   /* 1e9 – base for big‑integer words        */
#define MAX_WORDS 3500          /* enough for ≈ 31500 decimal digits       */
#define MAX_M     20            /* enough to reach ≈ 10000 digits of output */
#define BUF_SIZE  40000         /* maximum digits for one Fermat number    */

/* Multiply two big integers a and b (little‑endian, base BASE) into res.   */
static void multiply_big(const uint32_t *a, size_t len_a,
                         const uint32_t *b, size_t len_b,
                         uint32_t *res, size_t *res_len)
{
    for (size_t i = 0; i < len_a + len_b; ++i) res[i] = 0;
    for (size_t i = 0; i < len_a; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < len_b; ++j) {
            uint64_t cur = (uint64_t)a[i] * b[j] + res[i + j] + carry;
            res[i + j]      = (uint32_t)(cur % BASE);
            carry           = cur / BASE;
        }
        res[i + len_b] = (uint32_t)carry;
    }
    *res_len = len_a + len_b;
    while (*res_len > 1 && res[*res_len - 1] == 0) --(*res_len);
}

/* Add one to a big integer a (in place).                                 */
static void add_one_big(uint32_t *a, size_t *len)
{
    size_t i = 0;
    uint64_t carry = 1;
    while (carry && i < *len) {
        uint64_t cur = (uint64_t)a[i] + carry;
        a[i]           = (uint32_t)(cur % BASE);
        carry          = cur / BASE;
        ++i;
    }
    if (carry) {
        a[*len] = (uint32_t)carry;
        ++(*len);
    }
}

seq_t a(idx_t n)
{
    /* Static state: accumulated Fermat numbers.                         */
    static char fermat_str[MAX_M][BUF_SIZE];
    static size_t fermat_len[MAX_M];
    static idx_t cumulative[MAX_M];
    static size_t m_done = 0;

    /* Big integer for B = 2^(2^m) (little‑endian, base BASE).             */
    static uint32_t big[MAX_WORDS];
    static size_t   big_len = 0;

    if (big_len == 0) {          /* initialise B = 2 for m = 0 */
        big[0] = 2;
        big_len = 1;
    }

    /* Generate Fermat numbers until the cumulative digits cover n.       */
    while (m_done == 0 || cumulative[m_done - 1] < n) {
        size_t m = m_done;

        /* F = B + 1  */
        uint32_t temp[MAX_WORDS];
        size_t temp_len = big_len;
        memcpy(temp, big, big_len * sizeof(uint32_t));
        add_one_big(temp, &temp_len);

        /* Convert F to decimal string.                                   */
        char buf[BUF_SIZE];
        size_t dec_len;
        bigint_to_decimal(temp, temp_len, buf, &dec_len);

        /* Store the string and its length.                              */
        memcpy(fermat_str[m], buf, dec_len + 1);   /* include NUL */
        fermat_len[m] = dec_len;
        cumulative[m] = (m == 0) ? (idx_t)dec_len
                                : cumulative[m - 1] + (idx_t)dec_len;

        /* Prepare next B = B * B  (i.e., 2^(2^(m+1))).                */
        uint32_t prod[MAX_WORDS];
        size_t prod_len;
        multiply_big(big, big_len, big, big_len, prod, &prod_len);
        big_len = prod_len;
        memcpy(big, prod, prod_len * sizeof(uint32_t));

        ++m_done;
    }

    /* Locate the desired digit.                                          */
    size_t pos = (size_t)(n - 1);   /* zero‑based offset in the stream */
    size_t m = 0;
    while (m < m_done && pos >= fermat_len[m]) {
        pos -= fermat_len[m];
        ++m;
    }

    unsigned char digit_char = (unsigned char)fermat_str[m][pos];
    return (seq_t)(digit_char - '0');
}
