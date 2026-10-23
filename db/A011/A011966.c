/*
 * A011966  Third differences of Bell numbers.
 * https://oeis.org/A011966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n+3) – 3·Bell(n+2) + 3·Bell(n+1) – Bell(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *bell   = NULL;   /* array of already‑computed Bell numbers      */
static size_t bell_cap = 0;    /* current capacity of the bell array         */
static idx_t  bell_len = 0;    /* number of Bell numbers that are filled     */

static void _grow_bell_array(size_t new_cap)
{
    if (bell_cap >= new_cap) return;
    seq_t *new_bell = (seq_t*)realloc(bell, new_cap * sizeof(seq_t));
    if (!new_bell) abort();
    bell = new_bell;
    bell_cap = new_cap;
}

static void _ensure_bell(idx_t N)
{
    if (N < bell_len) return;

    if (bell_len == 0) {
        bell_cap = 1;
        bell = (seq_t*)malloc(bell_cap * sizeof(seq_t));
        if (!bell) abort();
        bell[0] = (seq_t)1;          /* B₀ = 1 */
        bell_len = 1;
    }

    if ((size_t)(N + 1) > bell_cap) _grow_bell_array((N + 1) * 2);

    for (idx_t m = bell_len; m <= N; ++m) {
        seq_t sum   = 0;
        seq_t coeff = (seq_t)1;      /* C(m‑1,0) */

        for (idx_t k = 0; k < m; ++k) {
            sum += coeff * bell[k];

            /* Update coeff to C(m‑1, k+1) */
            if (k + 1 < m) {
                coeff = coeff * (seq_t)(m - 1 - k);
                coeff = coeff / (seq_t)(k + 1);
            }
        }
        bell[m] = sum;
    }
    bell_len = N + 1;
}

seq_t a(idx_t n)
{
    _ensure_bell(n + 3);

    seq_t b0 = bell[n];
    seq_t b1 = bell[n + 1];
    seq_t b2 = bell[n + 2];
    seq_t b3 = bell[n + 3];

    /* a(n) = B_{n+3} – 3·B_{n+2} + 3·B_{n+1} – B_n */
    seq_t res = b3;
    res -= (seq_t)3 * b2;
    res += (seq_t)3 * b1;
    res -= b0;
    return res;
}
