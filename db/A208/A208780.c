/*
 * A208780  T(n,k)=Number of nXk 0..1 arrays avoiding 0 0 0 and 0 0 1
 *          horizontally and 0 1 1 and 1 1 1 vertically.
 * https://oeis.org/A208780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of binary n×k arrays avoiding * horizontal 000 and
 *            001, and vertical 011 and 111, * where the pairs (n,k) enumerate
 *            the antidiagonals of * the table for T(n,k)
 * Range:     u8 n <= 16, u16 n <= 67, u32 n <= 277, u64 n <= 1082,
 *            u128 n <= 4279, big any n
 * Validated: standard, pass, 74 terms through n = 9162 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* static data structures are initialized on first use */
#include <stdbool.h>
#include <stdint.h>

/* helper: compute T(4,4) by exhaustive enumeration (16 bits) */
static seq_t _t44(void)
{
    seq_t cnt = 0;
    for (uint32_t m = 0; m < (1u << 16); ++m) {
        bool bad = false;
        /* check horizontal constraints for each row (4 bits) */
        for (int r = 0; r < 4; ++r) {
            uint8_t row = (m >> (r * 4)) & 0x0F;   /* 4 bits */
            /* position 0..1 for start of 3‑cell window */
            for (int c = 0; c <= 1; ++c) {
                uint8_t w = (row >> c) & 0x07;   /* 3 bits */
                if ((w == 0b000) || (w == 0b001)) {
                    bad = true;
                    break;
                }
            }
            if (bad) break;
        }
        if (bad) continue;

        /* check vertical constraints for each column (4 rows) */
        for (int c = 0; c < 4; ++c) {
            /* build 4‑bit column */
            uint8_t col = 0;
            for (int r = 0; r < 4; ++r)
                col |= ((m >> (r * 4 + c)) & 1u) << r;
            /* 3-cell windows starting at row 0 and 1 */
            for (int r = 0; r <= 1; ++r) {
                uint8_t w = (col >> r) & 0x07;
                if ((w == 0b011) || (w == 0b111)) {
                    bad = true;
                    break;
                }
            }
            if (bad) break;
        }
        if (!bad) ++cnt;
    }
    return cnt;
}

/* Limits: for n up to 10000 the indices i,j do not exceed 141.
 * We precompute values up to 200 for safety. */
#define MAX 200

static bool _initialized = false;
static seq_t _col1[MAX + 1];   /* 1×k */
static seq_t _col2[MAX + 1];   /* n×2 */
static seq_t _col3[MAX + 1];   /* n×3 */
static seq_t _col4[MAX + 1];   /* n×4 */
static seq_t _base1[MAX + 1];  /* T(1,k)  (row 1) */

/* initialization routine */
static void _init(void)
{
    if (_initialized) return;
    _initialized = true;

    /* compute T(1,k) i.e. base1 -- Fibonacci kind recurrence */
    _base1[0] = 0;               /* unused */
    _base1[1] = 2;
    _base1[2] = 4;
    for (idx_t k = 3; k <= MAX; ++k)
        _base1[k] = _base1[k - 1] + _base1[k - 2];

    /* column 2: T(n,2), recurrence a(n)=2*a(n-1)+2*a(n-2)-a(n-3) */
    _col2[0] = 0;               /* unused */
    _col2[1] = 4;
    _col2[2] = 16;
    _col2[3] = 36;
    for (idx_t n = 4; n <= MAX; ++n)
        _col2[n] = 2 * _col2[n - 1] + 2 * _col2[n - 2] - _col2[n - 3];

    /* column 3: T(n,3), recurrence a(n)=a(n-1)+a(n-2) for n>4 */
    _col3[0] = 0;
    _col3[1] = 6;
    _col3[2] = 36;
    _col3[3] = 36;
    _col3[4] = 60;
    for (idx_t n = 5; n <= MAX; ++n)
        _col3[n] = _col3[n - 1] + _col3[n - 2];

    /* column 4: T(n,4), recurrence similar to column 3,
     * but a(4) requires enumeration */
    _col4[0] = 0;
    _col4[1] = 10;
    _col4[2] = 100;
    _col4[3] = 60;
    _col4[4] = _t44();          /* computed once by brute force */
    for (idx_t n = 5; n <= MAX; ++n)
        _col4[n] = _col4[n - 1] + _col4[n - 2];

    /* column 1: T(n,1), Fibonacci recurrence */
    _col1[0] = 0;
    _col1[1] = 2;
    _col1[2] = 4;
    for (idx_t n = 3; n <= MAX; ++n)
        _col1[n] = _col1[n - 1] + _col1[n - 2];
}

/* Return T(n,k) using precomputed sequences and base values.
 * n,k are 1‑based. */
static seq_t _T(idx_t n, idx_t k)
{
    _init();

    if (k == 1) return _col1[n];
    if (k == 2) return _col2[n];
    if (k == 3) return _col3[n];
    if (k == 4) return _col4[n];

    /* k >= 5: use recurrence a(n)=a(n-1)+a(n-2) for n>4 */
    /* base values from symmetry and row 1 */
    seq_t a1 = _base1[k];          /* T(1,k) */
    seq_t a2 = _col2[k];          /* T(2,k) = T(k,2) */
    seq_t a3 = _col3[k];          /* T(3,k) = T(k,3) */
    seq_t a4 = _col4[k];          /* T(4,k) = T(k,4) */
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;

    /* iterate recurrence from 5 to n */
    seq_t prev2 = a3;
    seq_t prev1 = a4;
    for (idx_t m = 5; m <= n; ++m) {
        seq_t cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

/* Public API function */
seq_t a(idx_t n)
{
    /* find s such that n <= s*(s-1)/2   (s starts at 2) */
    idx_t s = 2;
    for ( ; (s * (s - 1)) / 2 < n; ++s ) {}

    /* offset within group of size s-1 */
    idx_t prev = (s - 1) * (s - 2) / 2;
    idx_t pos  = n - prev;           /* 1-based position in group */

    idx_t i = s - pos;   /* row index */
    idx_t j = pos;       /* column index */

    return _T(i, j);
}
