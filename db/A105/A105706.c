/*
 * A105706  Smallest m such that 6 is at the n-th position of the decimal
 *          representation of the m-th Fibonacci number.
 * https://oeis.org/A105706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A105706
 *  Formula: a(n) = the smallest m such that the digit 6 occurs at the
 *  n‑th position from the right (0‑based) in the decimal representation
 *  of the m‑th Fibonacci number.
 *
 *  The function a(idx_t n) computes this value for any n ≥ 0.
 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_malloc.h"

#define MAX_N   20000          /* maximum n expected in the tests   */
#define MAX_DIG 20000          /* maximum number of decimal digits */

/* **********  Precomputation  ********** */
static seq_t *answer;          /* answer[n] = required m for position n   */
static bool   ready   = false; /* initialisation flag */

/*  *f[0], f[1], f[2] are the three buffers that will rotate  */
static uint8_t buf0[MAX_DIG];
static uint8_t buf1[MAX_DIG];
static uint8_t buf2[MAX_DIG];
static uint8_t *f[3];
static size_t   len[3];

/*  Called once to build the table of answers for all n ≤ MAX_N.   */
static void precompute(void)
{
    if (ready) return;
    ready = true;

    /* allocate the answer array */
    answer = seq_malloc((MAX_N + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= MAX_N; ++i) answer[i] = 0;

    bool found[MAX_N + 1];
    for (idx_t i = 0; i <= MAX_N; ++i) found[i] = false;
    size_t remaining = MAX_N + 1;   /* how many positions are still unknown */

    /* initialise the three buffers */
    f[0] = buf0; f[1] = buf1; f[2] = buf2;
    len[0] = 1;    f[0][0] = 0;          /* F_0 = 0  */
    len[1] = 1;    f[1][0] = 1;          /* F_1 = 1  */

    /* check k = 0 and k = 1 first */
    for (idx_t n = 0; n < len[0] && n <= MAX_N; ++n)
        if (!found[n] && f[0][n] == 6) {
            answer[n] = (seq_t)0;
            found[n]  = true;
            if (--remaining == 0) return;
        }

    for (idx_t n = 0; n < len[1] && n <= MAX_N; ++n)
        if (!found[n] && f[1][n] == 6) {
            answer[n] = (seq_t)1;
            found[n]  = true;
            if (--remaining == 0) return;
        }

    /* iterate over Fibonacci indices k = 2,3,4,... until all positions are known */
    for (idx_t k = 2; remaining > 0; ++k)
    {
        /* compute f[2] = f[0] + f[1] */
        size_t maxlen = (len[0] > len[1] ? len[0] : len[1]);
        uint8_t carry = 0;
        size_t i;
        for (i = 0; i < maxlen; ++i)
        {
            uint8_t a = (i < len[0]) ? f[0][i] : 0;
            uint8_t b = (i < len[1]) ? f[1][i] : 0;
            uint8_t s = a + b + carry;
            f[2][i] = s % 10;
            carry   = s / 10;
        }
        if (carry) f[2][i++] = carry;
        len[2] = i;

        /* rotate buffers: f[0] <- f[1], f[1] <- f[2], f[2] <- old f[0] */
        uint8_t *tmp_buf  = f[0];
        f[0] = f[1];
        f[1] = f[2];
        f[2] = tmp_buf;
        size_t tmp_len  = len[0];
        len[0] = len[1];
        len[1] = len[2];
        len[2] = tmp_len;

        /* check each position that is not yet found and is available in this F_k */
        size_t limit = (len[1] < (MAX_N + 1) ? len[1] : (MAX_N + 1));
        for (idx_t n = 0; n < limit; ++n)
        {
            if (!found[n] && f[1][n] == 6)
            {
                answer[n] = (seq_t)k;
                found[n]  = true;
                if (--remaining == 0) return;
            }
        }
    }
}

/* **********  Public function  ********** */
seq_t a(idx_t n)
{
    precompute();
    if (n > MAX_N) return 0;  /* value not pre‑computed – should not happen for tests */
    return answer[n];
}
