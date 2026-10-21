/*
 * A052681  Expansion of e.g.f. (1-x)/(1 - x - x^2 - 2*x^3 + 2*x^4).
 * https://oeis.org/A052681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Formula implementing A052681 via its recurrence:
   a(0)=1, a(1)=0, a(2)=2, a(3)=18
   a(n+4) = (n+4)*a(n+3) + (12+7*n+n^2)*a(n+2)
            + (48+52*n+18*n^2+2*n^3)*a(n+1)
            - 2*(n^4+10*n^3+35*n^2+50*n+24)*a(n)  */
#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#else
#include <stdint.h>
#endif

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)18;

#ifdef __cplusplus
    /* Use a static vector for memoisation (C++ mode) */
    static std::vector<seq_t> memo;
    if (memo.empty()) {
        memo.resize(4);
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)0;
        memo[2] = (seq_t)2;
        memo[3] = (seq_t)18;
    }
    while ((idx_t)memo.size() <= n) {
        idx_t k   = (idx_t)memo.size();    /* current index */
        idx_t idx = k - 4;                 /* corresponds to n in recurrence */
        seq_t c1 = (seq_t)k;
        seq_t c2 = (seq_t)(12 + 7*idx + idx*idx);
        seq_t c3 = (seq_t)(48 + 52*idx + 18*idx*idx + 2*idx*idx*idx);
        seq_t c4 = (seq_t)(2*(idx*idx*idx*idx
                              + 10*idx*idx*idx
                              + 35*idx*idx
                              + 50*idx
                              + 24));
        seq_t temp = c1 * memo[k-1] + c2 * memo[k-2] + c3 * memo[k-3];
        temp -= c4 * memo[k-4];
        memo.push_back(temp);
    }
    return memo[n];
#else
    /* Use a static dynamic array for memoisation (C mode) */
    static seq_t *memo = NULL;
    static idx_t memo_size  = 0;
    static idx_t memo_cap   = 0;

    /* initialise the table with the first four terms */
    if (memo == NULL) {
        memo_cap = 16; /* initial capacity > 4 */
        memo = (seq_t*)malloc(sizeof(seq_t) * memo_cap);
        if (!memo) return (seq_t)0; /* allocation failed; return 0 as safe fallback */
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)0;
        memo[2] = (seq_t)2;
        memo[3] = (seq_t)18;
        memo_size = 4;
    }

    /* grow the array if necessary */
    while (memo_size <= n) {
        if (memo_size >= memo_cap) {
            memo_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(memo, sizeof(seq_t) * memo_cap);
            if (!tmp) return (seq_t)0; /* allocation failed */
            memo = tmp;
        }
        idx_t k   = memo_size;          /* current index to compute */
        idx_t idx = k - 4;              /* corresponds to n in recurrence */
        seq_t c1 = (seq_t)k;
        seq_t c2 = (seq_t)(12 + 7*idx + idx*idx);
        seq_t c3 = (seq_t)(48 + 52*idx + 18*idx*idx + 2*idx*idx*idx);
        seq_t c4 = (seq_t)(2*(idx*idx*idx*idx
                              + 10*idx*idx*idx
                              + 35*idx*idx
                              + 50*idx
                              + 24));
        seq_t temp = c1 * memo[k-1] + c2 * memo[k-2] + c3 * memo[k-3];
        temp -= c4 * memo[k-4];
        memo[memo_size++] = temp;
    }

    return memo[n];
#endif
}
