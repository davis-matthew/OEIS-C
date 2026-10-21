/*
 * A084044  Smallest palindromic multiple of n in which n is a substring
 *          (anywhere), or 0 if n = 10k or no such number exists.
 * https://oeis.org/A084044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084044: smallest palindromic multiple of n in which n is a substring
 * (0 if n is a multiple of 10 or no such number exists). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_LEN 60   /* maximum palindrome length to search */

static uint64_t pow10_mod[MAX_LEN];   /* 10^k mod n for k=0..MAX_LEN-1 */

/* -------------------------------------------------------------------- */
/* helper: get decimal digits of n (most significant first) */
static void get_digits(idx_t n, int *digits, int *len)
{
    if (n == 0) {
        digits[0] = 0;
        *len = 1;
        return;
    }
    uint64_t tmp = n;
    int rev[16];
    int l = 0;
    while (tmp > 0) {
        rev[l++] = tmp % 10;
        tmp /= 10;
    }
    for (int i = 0; i < l; ++i)
        digits[i] = rev[l - 1 - i];
    *len = l;
}

/* -------------------------------------------------------------------- */
/* context for DFS search */
typedef struct {
    int *digits;            /* current digits array (most significant first) */
    const int *freePos;     /* indices of free (unknown) digits */
    const uint64_t *coeff;  /* coefficient of each free digit mod n */
    int freeCount;          /* number of free digits */
    int L;                  /* length of palindrome */
    uint64_t modN;          /* n as uint64_t */
} DFSContext;

/* DFS solver: try to assign digits to free positions so that remainder becomes 0 */
static bool dfs_solver(int depth, uint64_t sumRem, DFSContext *ctx)
{
    if (depth == ctx->freeCount) {
        return (sumRem % ctx->modN) == 0;
    }
    int pos = ctx->freePos[depth];
    int start = 0;
    if (pos == 0) start = 1;          /* leading digit cannot be 0 */
    int mirror = ctx->L - 1 - pos;
    for (int d = start; d <= 9; ++d) {
        ctx->digits[pos] = d;
        if (mirror != pos)
            ctx->digits[mirror] = d;
        uint64_t newRem = (sumRem + ctx->coeff[pos] * (uint64_t)d) % ctx->modN;
        if (dfs_solver(depth + 1, newRem, ctx))
            return true;
        ctx->digits[pos] = -1;
        if (mirror != pos)
            ctx->digits[mirror] = -1;
    }
    return false;
}

/* -------------------------------------------------------------------- */
/* main function implementing the sequence */
seq_t a(idx_t n)
{
    /* return 0 for multiples of 10 */
    if (n % 10 == 0)
        return 0;

    int nDigits[16];
    int lenN;
    get_digits(n, nDigits, &lenN);

    /* precompute powers of 10 modulo n */
    uint64_t modN = (uint64_t)n;
    pow10_mod[0] = 1 % modN;
    for (int i = 1; i < MAX_LEN; ++i)
        pow10_mod[i] = (pow10_mod[i - 1] * 10) % modN;

    seq_t bestVal = 0;
    bool foundAny = false;

    for (int L = lenN; L <= MAX_LEN; ++L) {
        /* search all possible start positions of the substring n */
        for (int p = 0; p <= L - lenN; ++p) {
            int digits[MAX_LEN];
            for (int i = 0; i < L; ++i)
                digits[i] = -1;
            /* place the digits of n */
            for (int i = 0; i < lenN; ++i)
                digits[p + i] = nDigits[i];

            /* propagate palindrome symmetry */
            bool conflict = false;
            bool changed = true;
            while (changed && !conflict) {
                changed = false;
                for (int i = 0; i < L; ++i) {
                    int j = L - 1 - i;
                    if (digits[i] != -1 && digits[j] == -1) {
                        digits[j] = digits[i];
                        changed = true;
                    } else if (digits[i] == -1 && digits[j] != -1) {
                        digits[i] = digits[j];
                        changed = true;
                    } else if (digits[i] != -1 && digits[j] != -1 && digits[i] != digits[j]) {
                        conflict = true;
                        break;
                    }
                }
            }
            if (conflict)
                continue;

            /* collect free positions (left half, including center if odd) */
            int freePos[MAX_LEN];
            int freeCount = 0;
            for (int i = 0; i <= (L - 1) / 2; ++i) {
                if (digits[i] == -1) {
                    freePos[freeCount++] = i;
                }
            }

            /* compute fixed sum modulo n and coefficients for free digits */
            uint64_t sum_fixed = 0;
            uint64_t coeff[MAX_LEN] = {0};
            for (int i = 0; i < L; ++i) {
                if (digits[i] != -1) {
                    uint64_t pw = pow10_mod[L - 1 - i];
                    sum_fixed = (sum_fixed + (uint64_t)digits[i] * pw) % modN;
                }
            }
            for (int k = 0; k < freeCount; ++k) {
                int pos = freePos[k];
                int mirror = L - 1 - pos;
                if (pos == mirror) {
                    coeff[pos] = pow10_mod[pos] % modN;
                } else {
                    coeff[pos] = (pow10_mod[L - 1 - pos] + pow10_mod[pos]) % modN;
                }
            }

            /* if no free digits, just check divisibility */
            if (freeCount == 0) {
                if (sum_fixed % modN == 0) {
                    seq_t cand = 0;
                    for (int i = 0; i < L; ++i)
                        cand = cand * (seq_t)10 + (seq_t)digits[i];
                    if (!foundAny || cand < bestVal) {
                        bestVal = cand;
                        foundAny = true;
                    }
                }
                continue;
            }

            /* DFS search for digits of free positions */
            DFSContext ctx = {digits, freePos, coeff, freeCount, L, modN};
            if (dfs_solver(0, sum_fixed % modN, &ctx)) {
                seq_t cand = 0;
                for (int i = 0; i < L; ++i)
                    cand = cand * (seq_t)10 + (seq_t)digits[i];
                if (!foundAny || cand < bestVal) {
                    bestVal = cand;
                    foundAny = true;
                }
            }
        }
        if (foundAny)
            return bestVal;
    }

    /* if no palindrome found within the search limit, return 0 */
    return 0;
}
