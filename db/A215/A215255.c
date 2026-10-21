/*
 * A215255  Let S be the binary string consisting of the first n+1 digits of
 *          (100101)*; a(n) = number of ways of writing S as a product of
 *          palindromes.
 * https://oeis.org/A215255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=3; for n≥4,
   a(n)=a(n-1)+a(n-d), where d=[2,4,2,4,3,3] for n%6=0..5
   (given by the OEIS entry for A215255). */
#ifndef __cplusplus
/* C implementation: simple static array, works for basic integer seq_t */
static seq_t memo_arr[20001];
static idx_t computed_upto = (idx_t)-1;
#else
/* C++ implementation: use std::vector to avoid huge static object */
#include <vector>
static std::vector<seq_t> memo_vec;
static idx_t computed_upto = 0;
#endif

static const idx_t d_table[6] = {2,4,2,4,3,3};

seq_t a(idx_t n) {
    /* initialise base cases on the first call */
#ifdef __cplusplus
    if (memo_vec.empty()) {
        memo_vec.resize(4);
        memo_vec[0] = (seq_t)1;
        memo_vec[1] = (seq_t)1;
        memo_vec[2] = (seq_t)2;
        memo_vec[3] = (seq_t)3;
        computed_upto = 3;
    }
#else
    if (computed_upto == (idx_t)-1) {
        memo_arr[0] = (seq_t)1;
        memo_arr[1] = (seq_t)1;
        memo_arr[2] = (seq_t)2;
        memo_arr[3] = (seq_t)3;
        computed_upto = 3;
    }
#endif

    /* answer immediately if already computed */
    if (n <= computed_upto) {
#ifdef __cplusplus
        return memo_vec[n];
#else
        return memo_arr[n];
#endif
    }

    /* extend the memoisation structure up to n */
#ifdef __cplusplus
    if (n >= memo_vec.size())
        memo_vec.resize(n + 1);
#endif

    for (idx_t i = computed_upto + 1; i <= n; ++i) {
        idx_t rem = i % 6;
        idx_t d   = d_table[rem];
#ifdef __cplusplus
        memo_vec[i] = memo_vec[i - 1] + memo_vec[i - d];
#else
        memo_arr[i] = memo_arr[i - 1] + memo_arr[i - d];
#endif
    }
    computed_upto = n;
#ifndef __cplusplus
    return memo_arr[n];
#else
    return memo_vec[n];
#endif
}
