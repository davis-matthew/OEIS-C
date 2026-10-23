/*
 * A045209  Numbers whose base-5 representation contains exactly one 0 and
 *          three 4's.
 * https://oeis.org/A045209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   pow3_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A045209: the numbers whose base‑5
 * representation contains exactly one digit 0 and exactly three digits 4.
 * The computation is done by explicitly enumerating the base‑5 strings
 * in lexicographic order (shorter strings first) and selecting the n‑th one.
 * The algorithm runs in O(L) per query, where L is the number of base‑5
 * digits of the answer (L ≤ 30 for n up to 10⁶).  All arithmetic is done
 * with types seq_t and idx_t; intermediate combinatorial counts are
 * capped at n to avoid overflow. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow3_cap.h"

/* ---------- helper routines ------------------------------------------------ */

/* Compute binomial coefficient C(n,k) with k ≤ 3 (small) using 64‑bit
 * integer arithmetic; returns 0 if k > n. */
static idx_t binom_nk(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;    /* integer arithmetic is exact */
    }
    return res;
}

/* Count how many base‑5 strings of length L satisfy the constraints:
 * exactly one '0', exactly three '4's, and a leading digit ≠ 0.
 * The result is capped at (cap+1). */
static idx_t count_len(idx_t L, idx_t cap)
{
    if (L < 4) return 0;                /* need at least 4 digits */
    /* zeros can be placed in any of the L‑1 positions (excluding msb) */
    idx_t zeros_pos = L - 1;
    idx_t choose_fours = binom_nk(L - 1, 3);
    if (choose_fours == 0) return 0;

    idx_t pow3 = pow3_cap(L - 4, cap);
    if (pow3 > cap) pow3 = cap + 1;

    /* zeros_pos * choose_fours * pow3, with overflow guard */
    idx_t res = zeros_pos;
    if (res > cap / choose_fours) res = cap + 1;
    else {
        res *= choose_fours;
        if (res > cap / pow3) res = cap + 1;
        else res *= pow3;
    }
    return res;
}

/* Count remaining completions for the suffix of length rem, given the
 * number of zeros (zn) and fours (fn) that still have to be used.
 * Result is capped at (cap+1). */
static idx_t count_remaining(idx_t rem, idx_t zn, idx_t fn, idx_t cap)
{
    if (zn > rem || fn > rem) return 0;
    if (zn + fn > rem) return 0;

    idx_t part = binom_nk(rem, zn);
    if (part == 0) return 0;
    part *= binom_nk(rem - zn, fn);
    if (part == 0) return 0;

    idx_t free = rem - zn - fn;
    idx_t pow3 = pow3_cap(free, cap);
    if (pow3 > cap) pow3 = cap + 1;

    if (part > cap / pow3) part = cap + 1;
    else part *= pow3;
    return part;
}

/* ---------- sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Find the length L of the base‑5 representation of the n‑th term. */
    idx_t cum = 0;
    idx_t L = 4;
    while (1) {
        idx_t cnt = count_len(L, n - cum + 1);
        if (cum + cnt >= n) break;
        cum += cnt;
        ++L;
    }
    idx_t k = n - cum;                 /* 1‑based index within length L */

    /* Construct the k‑th lexicographic base‑5 string of length L. */
    uint8_t digits[128];
    idx_t zeros_needed = 1;
    idx_t fours_needed = 3;

    for (idx_t pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 4; ++d) {
            if (pos == 0 && d == 0) continue;           /* no leading zero */
            if (d == 0 && zeros_needed == 0) continue;  /* no more zeros */
            if (d == 4 && fours_needed == 0) continue;  /* no more fours */

            idx_t zn = zeros_needed - (d == 0);
            idx_t fn = fours_needed - (d == 4);
            idx_t rem = L - pos - 1;
            idx_t cnt_rem = count_remaining(rem, zn, fn, n);

            if (k > cnt_rem) {
                k -= cnt_rem;
                continue;
            } else {
                digits[pos] = (uint8_t)d;
                zeros_needed = zn;
                fours_needed = fn;
                break;
            }
        }
    }

    /* Convert the base‑5 digit array to the numeric value. */
    seq_t value = 0;
    for (idx_t i = 0; i < L; ++i) {
        value = value * 5 + digits[i];
    }
    return value;
}
