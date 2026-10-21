/*
 * A074306  Inverse of permutation in A074305.
 * https://oeis.org/A074306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-4) - a(n-6) Initial values: a(0)=0, a(1)=2,
 *            a(2)=3, a(3)=1, a(4)=6, a(5)=5 The implementation stores already
 *            computed values and grows the storage on demand. It works in
 *            both C11 and C++17
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* ----- constants ----- */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)5;

#ifdef __cplusplus
    /* ----- C++ implementation using std::vector ----- */
    static std::vector<seq_t> seq;
    static bool init = false;
    if (!init) {
        seq.resize(6);
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)3;
        seq[3] = (seq_t)1;
        seq[4] = (seq_t)6;
        seq[5] = (seq_t)5;
        init = true;
    }
    if ((size_t)n >= seq.size()) {
        size_t cur = seq.size();
        seq.resize((size_t)n + 1);
        for (size_t i = cur; i <= (size_t)n; ++i) {
            seq[i] = seq[i-2] + seq[i-4] - seq[i-6];
        }
    }
    return seq[(size_t)n];
#else
    /* ----- C implementation using malloc/realloc ----- */
    static seq_t *seq = NULL;
    static size_t len = 0;   /* number of elements stored */
    static size_t cap = 0;   /* allocated capacity */

    /* Ensure base values are stored if not already done */
    if (len == 0) {
        cap = 16;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)2;
        seq[2] = (seq_t)3;
        seq[3] = (seq_t)1;
        seq[4] = (seq_t)6;
        seq[5] = (seq_t)5;
        len = 6;
    }

    /* Grow storage as needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = len; i <= n; ++i) {
        seq[i] = seq[i-2] + seq[i-4] - seq[i-6];
    }
    len = (size_t)n + 1;

    return seq[(size_t)n];
#endif
}
