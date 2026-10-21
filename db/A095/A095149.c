/*
 * A095149  Triangle read by rows: Aitken's array (A011971) but with a leading
 *          diagonal before it given by the Bell numbers (A000110), 1, 1, 2,
 *          5, 15, 52, ...
 * https://oeis.org/A095149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   T(r,k) = Σ_{i=0}^{k-2} C(k-2,i)*B(r-2-i) (for 3≤k≤r) a(n) is the
 *            flattening of the triangle row‑by‑row, starting from
 *            a(0)=T(1,1)=1. B(m) are the Bell numbers (B(0)=1). This
 *            implementation precomputes Bell numbers on demand; no dynamic
 *            memory allocation is required because the tests only ask for...
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 135, u64 n <= 350,
 *            u128 n <= 945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* maximum Bell index needed for n≤10000 is 142; give margin */
#define MAX_BELL 200

/* static cache for Bell numbers */
static seq_t bell[MAX_BELL + 1];
static idx_t bell_size = 0;          /* number of Bell numbers currently computed */

/* initialise Bell[0] = 1 */
static inline void init_bell(void)
{
    if (bell_size == 0) {
        bell[0] = (seq_t)1;
        bell_size = 1;
    }
}

/* multiplicative binomial coefficient C(n,k), result in seq_t */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    }
    return r;
}

/* ensure Bell numbers up to B(max_index) are computed */
static void ensure_bell(idx_t max_index)
{
    if (bell_size > 0 && max_index < bell_size) return;
    if (max_index >= MAX_BELL) max_index = MAX_BELL - 1; /* safeguard */
    init_bell();
    for (idx_t i = bell_size; i <= max_index; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j < i; ++j) {
            sum += binom((idx_t)i - 1, (idx_t)j) * bell[j];
        }
        bell[i] = sum;
    }
    bell_size = max_index + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find row r (1‑based) such that r*(r+1)/2 > n */
    idx_t low = 1;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 v = (unsigned __int128)mid * (mid + 1) / 2;
        if (v > n) high = mid; else low = mid + 1;
    }
    idx_t r = low; /* 1‑based row number */

    /* column within row (1‑based) */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev_tri + 1;

    /* compute necessary Bell numbers */
    if (r >= 1) ensure_bell(r - 1);
    if (c == 1) return bell[r - 1];          /* T(r,1) = B(r-1) */
    if (c == 2) {
        if (r < 2) return (seq_t)0;          /* impossible for r=1 */
        return bell[r - 2];                  /* T(r,2) = B(r-2) */
    }

    /* T(r,k) for k >= 3: Σ_{i=0}^{k-2} C(k-2,i) * B(r-2-i) */
    idx_t k = c;
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i <= k - 2; ++i) {
        sum += binom(k - 2, i) * bell[r - 2 - i];
    }
    return sum;
}
