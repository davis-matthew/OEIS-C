/*
 * A033068  Numbers with the property that all pairs of consecutive base-3
 *          digits differ by 1.
 * https://oeis.org/A033068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 155, u32 n <= 5627, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerates base‑3 strings with consecutive digits differing by 1.
   a(n) is the nth such integer in increasing order. */

#include <stdint.h>
#include <stdbool.h>

enum { MAX_LEN = 64 };                     /* enough for n up to many thousands */

static bool tables_inited = false;
static idx_t f[MAX_LEN+1][3];
static idx_t A[MAX_LEN+1];
static idx_t cum[MAX_LEN+1];
static idx_t suffix[MAX_LEN+1][3];

static void init_tables(void)
{
    if (tables_inited) return;
    tables_inited = true;

    /* base case: length 1 (first digit cannot be 0) */
    f[1][0] = 0;
    f[1][1] = 1;
    f[1][2] = 1;
    A[1] = f[1][0] + f[1][1] + f[1][2];   /* =2 */
    cum[1] = A[1];

    for (int L = 2; L <= MAX_LEN; ++L) {
        for (int d = 0; d < 3; ++d) {
            idx_t v = 0;
            if (d > 0) v += f[L-1][d-1];
            if (d < 2) v += f[L-1][d+1];
            f[L][d] = v;
        }
        A[L] = f[L][0] + f[L][1] + f[L][2];
        cum[L] = cum[L-1] + A[L];
    }

    /* suffix table: number of ways to complete 'rem' positions after previous digit 'd' */
    for (int d = 0; d < 3; ++d) suffix[0][d] = 1;          /* no positions left */
    for (int rem = 1; rem <= MAX_LEN; ++rem) {
        for (int d = 0; d < 3; ++d) {
            idx_t v = 0;
            if (d > 0) v += suffix[rem-1][d-1];
            if (d < 2) v += suffix[rem-1][d+1];
            suffix[rem][d] = v;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined below the offset */

    init_tables();

    /* find length L such that n lies within cum[L] */
    int L = 1;
    while (L <= MAX_LEN && n > cum[L]) ++L;

    idx_t prev_cum = (L > 1) ? cum[L-1] : 0;
    idx_t offset   = n - prev_cum - 1;   /* zero‑based index within this length */

    int digits[MAX_LEN];
    int prev_digit = -1; /* sentinel before first position */

    for (int pos = 0; pos < L; ++pos) {
        int remaining = L - pos - 1;
        int choices[3];
        unsigned csz = 0;

        /* allowed digits in ascending order */
        if (prev_digit == -1) {           /* first digit: cannot be 0 */
            choices[0] = 1;
            choices[1] = 2;
            csz = 2;
        } else if (prev_digit == 0) {
            choices[0] = 1;
            csz = 1;
        } else if (prev_digit == 1) {
            choices[0] = 0;
            choices[1] = 2;
            csz = 2;
        } else { /* prev_digit == 2 */
            choices[0] = 1;
            csz = 1;
        }

        int chosen = 0;
        for (unsigned i = 0; i < csz; ++i) {
            int d = choices[i];
            idx_t cnt = suffix[remaining][d];
            if (offset < cnt) {
                chosen = d;
                break;
            }
            offset -= cnt;
        }

        digits[pos] = chosen;
        prev_digit = chosen;
    }

    seq_t value = 0;
    for (int pos = 0; pos < L; ++pos)
        value = value * 3 + (seq_t)digits[pos];

    return value;
}
