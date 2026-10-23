/*
 * A043672  Numbers whose base-15 representation has exactly 5 runs.
 * https://oeis.org/A043672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n-th integer (n ≥ 1) whose base‑15 representation has exactly five runs. */

#include <stddef.h>
#include <stdlib.h>

/* ---------- helper utilities ---------- */
static inline idx_t
max_idx_t(void)
{
    return (idx_t)-1;   /* maximum value of unsigned idx_t */
}

static inline idx_t
cap_add(idx_t a, idx_t b)
{
    idx_t maxv = max_idx_t();
    if (a > maxv - b)
        return maxv;
    return a + b;
}

/* compute C(L-1,4) for L ≥ 5, result in idx_t (clamped to max). */
static idx_t
binom5(idx_t L)
{
    if (L < 5)
        return 0;
    idx_t a = L - 1;
    idx_t b = L - 2;
    idx_t c = L - 3;
    idx_t d = L - 4;
    /* divide by 2,3,4 individually to keep numbers small */
    uint64_t arr[4] = { a, b, c, d };
    uint64_t divs[3] = { 2, 3, 4 };
    for (size_t k = 0; k < 3; k++) {
        uint64_t div = divs[k];
        for (size_t i = 0; i < 4; i++) {
            if (arr[i] % div == 0) {
                arr[i] /= div;
                break;
            }
        }
    }
    __uint128_t prod = (__uint128_t)arr[0] * arr[1];
    prod *= arr[2];
    prod *= arr[3];
    if (prod > max_idx_t())
        return max_idx_t();
    return (idx_t)prod;
}

/* count of length‑L numbers with exactly five runs */
static idx_t
count_len(idx_t L)
{
    static const idx_t POW14_5 = 537824;   /* 14^5 */
    idx_t c = binom5(L);                 /* C(L-1,4) */
    __uint128_t prod = (__uint128_t)c * POW14_5;
    if (prod > max_idx_t())
        return max_idx_t();
    return (idx_t)prod;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, return 0 as a safe default */

    /* 1. find the length L of the n‑th number */
    idx_t cumulative = 0;
    idx_t L = 5;
    while (1) {
        idx_t cnt = count_len(L);
        if (cumulative + cnt >= n)
            break;
        cumulative += cnt;
        L++;
    }

    /* 2. offset inside the block of numbers of length L (0‑based) */
    idx_t offset = n - cumulative - 1;

    /* 3. pre‑compute DP[rem][prev][runs] for rem = 0..L */
    size_t rows = L + 1;
    size_t nprev = 15, nruns = 6;              /* prev digit 0..14, runs 0..5 */
    size_t stride = nprev * nruns;
    size_t dp_size = rows * stride;
    /* use malloc to avoid large stack usage */
    idx_t *dp = (idx_t *)calloc(dp_size, sizeof(idx_t));
    if (!dp)
        return 0;   /* out of memory – unlikely for the problem size */

    /* base: no remaining digits */
    for (size_t prev = 0; prev < nprev; prev++) {
        for (size_t r = 0; r < nruns; r++) {
            dp[prev * nruns + r] = (r == 5) ? 1 : 0;
        }
    }

    /* DP for remaining positions */
    for (size_t rem = 1; rem <= L; rem++) {
        size_t base = rem * stride;
        size_t prev_base = (rem - 1) * stride;
        for (size_t prev = 0; prev < nprev; prev++) {
            for (size_t r = 0; r < nruns; r++) {
                idx_t sum = 0;
                for (size_t d = 0; d < nprev; d++) {
                    size_t nr = r + (d != prev);
                    if (nr > 5) continue;
                    sum = cap_add(sum, dp[prev_base + d * nruns + nr]);
                }
                dp[base + prev * nruns + r] = sum;
            }
        }
    }

    /* 4. build the number digit by digit */
    seq_t result = 0;
    int prevDigit = -1;
    int runs = 0;           /* runs used so far (including current run) */

    for (size_t pos = 0; pos < L; pos++) {
        int start = (pos == 0) ? 1 : 0;
        size_t rem = L - pos - 1;
        size_t bp = rem * stride;       /* base index for rem */

        for (int d = start; d < 15; d++) {
            int newRuns = runs;
            if (prevDigit == -1)
                newRuns = 1;
            else if (d != prevDigit)
                newRuns = runs + 1;

            if (newRuns > 5)
                continue;

            idx_t cnt = dp[bp + d * nruns + newRuns];
            if (cnt <= offset) {
                offset -= cnt;
                continue;
            }

            /* choose this digit */
            result = result * 15 + d;
            prevDigit = d;
            runs = newRuns;
            break;
        }
    }

    free(dp);
    return result;
}
