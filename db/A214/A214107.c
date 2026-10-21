/*
 * A214107  Number of 0..3 colorings on an n X 4 array circular in the 4
 *          direction with new values 0..3 introduced in row major order.
 * https://oeis.org/A214107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*a(n-1) -107*a(n-2) +73*a(n-3) (n ≥ 4) with a(1) = 4,
 *            a(2) = 111, a(3) = 3502
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_CACHE 20000   /* sufficient for n up to ~10 000 */

static seq_t cache[MAX_CACHE + 1];
static idx_t computed_high = 0;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below offset */

    /* Use cached values for small n */
    if (n <= (idx_t)MAX_CACHE) {
        /* Initialise base values on first use */
        if (computed_high < 3) {
            cache[1] = (seq_t)4;
            cache[2] = (seq_t)111;
            cache[3] = (seq_t)3502;
            computed_high = 3;
        }

        /* Compute any missing values up to n */
        if (n > computed_high) {
            for (idx_t i = computed_high + 1; i <= n; ++i) {
                cache[i] =
                    (seq_t)35 * cache[i - 1]
                  - (seq_t)107 * cache[i - 2]
                  + (seq_t)73 * cache[i - 3];
            }
            computed_high = n;
        }

        return cache[n];
    }

    /* For large n compute the value iteratively (no cache) */
    seq_t a1 = (seq_t)4;    /* a(1) */
    seq_t a2 = (seq_t)111;  /* a(2) */
    seq_t a3 = (seq_t)3502; /* a(3) */

    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    seq_t next;
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)35 * a3
             - (seq_t)107 * a2
             + (seq_t)73 * a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
