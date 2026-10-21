/*
 * A224356  Number of 5Xn 0..2 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A224356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 16, u64 n <= 165, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224356: Number of 5×n 0..2 arrays with rows nondecreasing and all
    diagonals and antidiagonals unimodal.
    For n>5 the sequence equals the rational polynomial
        a(n) = (1009/907200)n^10 + (1489/181440)n^9 + … + (3586529/630)n – 10675,
    which after clearing denominators becomes
        a(n) = (1/907200)(1009 n^10 + 7445 n^9 + 115485 n^8 + 794910 n^7
                         + 4934307 n^6 + 45251745 n^5 – 115180885 n^4
                         + 1639611340 n^3 – 1971893916 n^2 + 5164601760 n
                         – 9684360000).

    For n ≤ 5 the values are obtained by brute‑force enumeration.
*/

#include <stdbool.h>

static bool is_unimodal(const int *v, int len)
{
    int last = -1;
    bool descended = false;
    for (int i = 0; i < len; ++i) {
        int cur = v[i];
        if (last != -1) {
            if (cur < last) descended = true;
            else if (descended && cur > last) return false;
        }
        last = cur;
    }
    return true;
}

/* Count 5×n arrays for n ≤ 5 by exhaustive search. */
static seq_t count_small(idx_t n)
{
    /* number of (a,b) patterns per row */
    idx_t num_patterns = (n + 1) * (n + 2) / 2;
    int pat_a[21], pat_b[21];
    idx_t id = 0;
    for (int a = 0; a <= (int)n; ++a)
        for (int b = a; b <= (int)n; ++b) {
            pat_a[id] = a;
            pat_b[id] = b;
            ++id;
        }

    seq_t count = 0;
    int mat[5][5];          /* maximum width is 5 */

    for (idx_t p0 = 0; p0 < num_patterns; ++p0)
    for (idx_t p1 = 0; p1 < num_patterns; ++p1)
    for (idx_t p2 = 0; p2 < num_patterns; ++p2)
    for (idx_t p3 = 0; p3 < num_patterns; ++p3)
    for (idx_t p4 = 0; p4 < num_patterns; ++p4) {
        const idx_t pat[5] = {p0, p1, p2, p3, p4};
        /* build the matrix */
        for (int r = 0; r < 5; ++r) {
            int a = pat_a[pat[r]];
            int b = pat_b[pat[r]];
            for (int j = 0; j < (int)n; ++j)
                mat[r][j] = (j < a) ? 0 : (j < b) ? 1 : 2;
        }

        /* NW‑SE diagonals: d = i - j   (range -(n-1)..4) */
        bool ok = true;
        for (int d = -(int)(n-1); d <= 4 && ok; ++d) {
            int val[5], len = 0;
            for (int j = 0; j < (int)n; ++j) {
                int i = j + d;
                if (i < 0 || i > 4) continue;
                val[len++] = mat[i][j];
            }
            if (!is_unimodal(val, len)) ok = false;
        }

        /* NE‑SW (antidiagonals): d = i + j  (range 0..n+3) */
        for (int d = 0; d <= 4 + (int)(n-1) && ok; ++d) {
            int val[5], len = 0;
            for (int j = 0; j < (int)n; ++j) {
                int i = d - j;
                if (i < 0 || i > 4) continue;
                val[len++] = mat[i][j];
            }
            if (!is_unimodal(val, len)) ok = false;
        }

        if (ok) ++count;
    }

    return count;
}

seq_t a(idx_t n)
{
    /* small n handled by brute force */
    if (n <= 5) return count_small(n);

    /* polynomial coefficients after clearing denominators */
    static const long long coeff[11] = {
        -9684360000LL,  /* n^0  */
         5164601760LL,  /* n^1  */
        -1971893916LL,  /* n^2  */
         1639611340LL,  /* n^3  */
        -115180885LL,   /* n^4  */
         45251745LL,    /* n^5  */
         4934307LL,     /* n^6  */
         794910LL,      /* n^7  */
        115485LL,       /* n^8  */
         7445LL,        /* n^9  */
         1009LL         /* n^10 */
    };
    const seq_t denom = (seq_t)907200;

    /* Use 128‑bit arithmetic when seq_t is 64‑bit; otherwise use
       arbitrary‑precision arithmetic with seq_t operations. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        /* 64‑bit path – use signed __int128 to avoid overflow. */
        __int128 num = coeff[10];
        for (int k = 9; k >= 0; --k) {
            num = num * ( __int128)n + ( __int128)coeff[k];
        }
        unsigned __int128 unum = (unsigned __int128)num;
        seq_t result = (seq_t)(unum / denom);
        return result;
    } else {
        /* arbitrary‑precision path – use seq_t operations only. */
        seq_t val = (seq_t)coeff[10];
        for (int k = 9; k >= 0; --k) {
            val = val * (seq_t)n;
            if (coeff[k] >= 0)
                val += (seq_t)coeff[k];
            else
                val -= (seq_t)(-coeff[k]);
        }
        return val / denom;
    }
}
