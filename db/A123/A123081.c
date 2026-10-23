/*
 * A123081  Infinite square array read by antidiagonals: T(n,k) = Bell(n+k) =
 *          A000110(n+k).
 * https://oeis.org/A123081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 135, u64 n <= 350,
 *            u128 n <= 945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A123081
 *  a(n) = Bell( ⌊ (√(8n+1) – 1) / 2 ⌋ )
 *  where Bell(m) is the m‑th Bell number, Bell(0)=1.  */

#include <stdlib.h>

static inline idx_t floor_sqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;            /* 2⁶⁴–1, largest 64‑bit value */
    idx_t best = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

/* static storage for Bell numbers */
static seq_t *bell = NULL;
static idx_t bell_cur = 0;          /* greatest index computed (inclusive) */
static size_t bell_cap = 0;         /* allocated capacity */

/* ensure capacity for Bell numbers up to 'need' (inclusive) */
static void ensure_bell_capacity(idx_t need)
{
    if (bell_cap <= (size_t)need) {
        size_t new_cap = bell_cap ? bell_cap * 2 : 16;
        if (new_cap <= (size_t)need) new_cap = (size_t)need + 1;
        seq_t *tmp = (seq_t *)realloc(bell, new_cap * sizeof(seq_t));
        bell = tmp;
        bell_cap = new_cap;
    }
}

/* compute Bell numbers up to index 'm' (inclusive) */
static void compute_bell_up_to(idx_t m)
{
    if (bell == NULL) {                     /* first time initialisation */
        bell = (seq_t *)malloc(sizeof(seq_t));
        bell[0] = (seq_t)1;                 /* Bell(0) = 1 */
        bell_cap = 1;
        bell_cur = 0;
    }
    if (m <= bell_cur) return;              /* already computed */

    ensure_bell_capacity(m);
    for (idx_t i = bell_cur + 1; i <= m; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;              /* binom(i-1,0) = 1 */
        idx_t n = i;
        for (idx_t k = 0; k < i; ++k) {
            sum += binom * bell[k];
            if (k + 1 < i) {
                binom = binom * (seq_t)(n - 1 - k) / (seq_t)(k + 1);
            }
        }
        bell[i] = sum;
    }
    bell_cur = m;
}

/* public function for the OEIS sequence */
seq_t a(idx_t n)
{
    /* compute diagonal index d = floor( (sqrt(8*n+1)-1)/2 ) */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    idx_t s = floor_sqrt_u128(val);
    idx_t d = (s - 1) / 2;

    compute_bell_up_to(d);
    return bell[d];
}
