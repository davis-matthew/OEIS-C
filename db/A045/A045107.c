/*
 * A045107  Numbers whose base-4 representation contains exactly four 1's and
 *          two 2's.
 * https://oeis.org/A045107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   choose_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045107: numbers whose base‑4 representation contains exactly
   four 1's and two 2's.  The implementation constructs the n‑th such
   integer by combinatorial counting of admissible digits.  This is not a
   simple closed formula, so the algorithm enumerates digit positions. */

#include <stdint.h>
#include "../utils/choose_uint.h"

/* powers of two up to 2^63 */
static const seq_t pow2_arr[64] = {
    1ULL, 2ULL, 4ULL, 8ULL, 16ULL, 32ULL, 64ULL, 128ULL,
    256ULL, 512ULL, 1024ULL, 2048ULL, 4096ULL, 8192ULL,
    16384ULL, 32768ULL, 65536ULL, 131072ULL, 262144ULL,
    524288ULL, 1048576ULL, 2097152ULL, 4194304ULL, 8388608ULL,
    16777216ULL, 33554432ULL, 67108864ULL, 134217728ULL,
    268435456ULL, 536870912ULL, 1073741824ULL, 2147483648ULL,
    4294967296ULL, 8589934592ULL, 17179869184ULL, 34359738368ULL,
    68719476736ULL, 137438953472ULL, 274877906944ULL, 549755813888ULL,
    1099511627776ULL, 2199023255552ULL, 4398046511104ULL,
    8796093022208ULL, 17592186044416ULL, 35184372088832ULL,
    70368744177664ULL, 140737488355328ULL, 281474976710656ULL,
    562949953421312ULL, 1125899906842624ULL, 2251799813685248ULL,
    4503599627370496ULL, 9007199254740992ULL, 18014398509481984ULL,
    36028797018963968ULL, 72057594037927936ULL, 144115188075855872ULL,
    288230376151711744ULL, 576460752303423488ULL, 1152921504606846976ULL,
    2305843009213693952ULL, 4611686018427387904ULL, 9223372036854775808ULL
};

/* Count how many completions exist for r remaining positions,
   needing rem1 ones and rem2 twos among them.  Remaining
   digits may be 0 or 3. */
static inline seq_t count_suffix(int r, int rem1, int rem2)
{
    if (rem1 < 0 || rem2 < 0 || rem1 + rem2 > r) return 0;
    seq_t cnt = choose_uint(r, rem1);
    cnt *= choose_uint(r - rem1, rem2);
    int other = r - rem1 - rem2;
    cnt *= pow2_arr[other];
    return cnt;
}

/* Count how many L‑digit numbers exist (most significant digit ≠ 0). */
static inline seq_t count_length(int L)
{
    seq_t total = 0;
    /* first digit = 1 */
    if (L - 1 >= 5) {
        seq_t c = choose_uint(L - 1, 3);
        c *= choose_uint(L - 1 - 3, 2);
        c *= pow2_arr[L - 6];
        total += c;
    }
    /* first digit = 2 */
    if (L - 1 >= 5) {
        seq_t c = choose_uint(L - 1, 4);
        c *= choose_uint(L - 1 - 4, 1);
        c *= pow2_arr[L - 6];
        total += c;
    }
    /* first digit = 3 */
    if (L - 1 >= 6) {
        seq_t c = choose_uint(L - 1, 4);
        c *= choose_uint(L - 1 - 4, 2);
        c *= pow2_arr[L - 7];
        total += c;
    }
    return total;
}

seq_t a(idx_t n)
{
    idx_t need = n;                     /* 1‑based index into the sequence */
    int L = 6;                          /* minimal number of base‑4 digits */

    /* Find the digit length containing the desired element. */
    for (;;) {
        seq_t cnt = count_length(L);
        if (need > (idx_t)cnt) {
            need -= (idx_t)cnt;
        } else {
            break;
        }
        ++L;
    }

    /* Build the digits. */
    int digits[64] = {0};
    int rem1 = 4, rem2 = 2;             /* remaining 1's and 2's to place */

    for (int pos = 0; pos < L; ++pos) {
        for (int d = 0; d <= 3; ++d) {
            if (pos == 0 && d == 0) continue;            /* no leading 0 */

            int r1 = rem1, r2 = rem2;
            if (d == 1) {
                if (r1 == 0) continue;
                --r1;
            } else if (d == 2) {
                if (r2 == 0) continue;
                --r2;
            }

            seq_t possible = count_suffix(L - pos - 1, r1, r2);
            if (need > (idx_t)possible) {
                need -= (idx_t)possible;
                continue;
            }

            digits[pos] = d;
            rem1 = r1;
            rem2 = r2;
            break;
        }
    }

    /* Convert base‑4 digits to decimal. */
    seq_t value = 0;
    for (int i = 0; i < L; ++i) {
        value = value * 4 + (seq_t)digits[i];
    }

    return value;
}
