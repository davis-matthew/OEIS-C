/*
 * A048246  Number of distinct 3-colorings (X, O and blank) of 3 X 3 grid with
 *          floor((n+1)/2) X's, floor(n/2) O's and 9 - n blanks.
 * https://oeis.org/A048246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1/8 * Σ_{g∈D4} |{colorings fixed by g with floor((n+1)/2)
 *            X's, floor(n/2) O's, * and 9−n blanks}| (Burnside’s lemma for
 *            the 3×3 tic‑tac‑toe board)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 10 terms through n = 9 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_fixed(int n, const int *len, int cnt) {
    int xTarget = (n + 1) / 2;
    int oTarget = n / 2;
    int bTarget = 9 - n;
    int total = 1;
    for (int i = 0; i < cnt; ++i) total *= 3;      /* 3^cnt assignments */

    seq_t res = 0;
    for (int a = 0; a < total; ++a) {
        int tmp = a;
        int cx = 0, co = 0, cb = 0;
        for (int j = 0; j < cnt; ++j) {
            int col = tmp % 3;     /* 0 = X, 1 = O, 2 = blank */
            tmp /= 3;
            int l = len[j];
            if (col == 0) cx += l;
            else if (col == 1) co += l;
            else cb += l;
        }
        if (cx == xTarget && co == oTarget && cb == bTarget) ++res;
    }
    return res;
}
static const int cycles_id[]      = {1,1,1,1,1,1,1,1,1};
static const int cycles_r90[]     = {4,4,1};
static const int cycles_r270[]    = {4,4,1};
static const int cycles_r180[]    = {2,2,2,2,1};
static const int cycles_vert[]    = {2,2,2,1,1,1};
static const int cycles_horz[]    = {2,2,2,1,1,1};
static const int cycles_diag[]    = {1,1,1,2,2,2};
static const int cycles_adiag[]   = {1,1,1,2,2,2};

static const int * const cycle_lists[8] = {
    cycles_id, cycles_r90, cycles_r180, cycles_r270,
    cycles_vert, cycles_horz, cycles_diag, cycles_adiag
};
static const int cycle_counts[8] = {9,3,5,3,6,6,6,6};

seq_t a(idx_t n) {
    if (n > 9u) return 0;                 /* more moves than cells → impossible */
    int ni = (int)n;
    seq_t sum = 0;
    for (int g = 0; g < 8; ++g)
        sum += compute_fixed(ni, cycle_lists[g], cycle_counts[g]);
    return sum / 8;                         /* divide by |D4| = 8 */
}
