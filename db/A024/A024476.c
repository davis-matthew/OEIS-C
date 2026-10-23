/*
 * A024476  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = (Lucas numbers), t = A023533.
 * https://oeis.org/A024476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n+1)/2)} A000032(k) * A023533(n-k+1)
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 101, u64 n <= 211,
 *            u128 n <= 403, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A023533(m) = 1 if m is a tetrahedral number, 0 otherwise */

#include <stdlib.h>

static seq_t *lucas = NULL;          /* 1‑based indexing: lucas[1] = 1, lucas[2] = 3   */
static idx_t lucas_cap = 0;          /* current allocated size (≥ needed)            */

static idx_t *tetra = NULL;          /* list of tetrahedral numbers up to current max  */
static idx_t tetra_len = 0;          /* number of elements stored in tetra[]          */
static idx_t tetra_cap = 0;          /* current capacity of tetra[]                  */
static idx_t next_tetra_m = 1;       /* next m to generate tetra(m)                  */
static idx_t next_tetra_val = 1;     /* value of tetra(next_tetra_m)                 */

/* Ensure that lucas array can access indices up to maxk (1‑based). */
static void ensure_lucas(idx_t maxk)
{
    if (lucas_cap >= maxk)
        return;

    idx_t new_cap = lucas_cap ? lucas_cap * 2 : 2;
    if (new_cap < maxk)
        new_cap = maxk;

    seq_t *new_lucas = (seq_t *)realloc(lucas, (new_cap + 1) * sizeof(seq_t));
    if (!new_lucas)
        exit(EXIT_FAILURE);
    lucas = new_lucas;

    /* Initialize the first two Lucas numbers on first allocation. */
    if (lucas_cap == 0) {
        lucas[1] = (seq_t)1;
        lucas[2] = (seq_t)3;
    }

    /* Compute new entries (if any). */
    idx_t start = lucas_cap ? lucas_cap + 1 : 3;
    for (idx_t i = start; i <= new_cap; ++i) {
        lucas[i] = lucas[i - 1] + lucas[i - 2];
    }

    lucas_cap = new_cap;
}

/* Append a tetrahedral number to the tetra[] list. */
static void add_tetra(idx_t val)
{
    if (tetra_len == 0) {
        tetra_cap = 16;
        tetra = (idx_t *)malloc(tetra_cap * sizeof(idx_t));
        if (!tetra)
            exit(EXIT_FAILURE);
    } else if (tetra_len == tetra_cap) {
        tetra_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(tetra, tetra_cap * sizeof(idx_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        tetra = tmp;
    }
    tetra[tetra_len++] = val;
}

/* Ensure tetra list contains all tetrahedral numbers ≤ maxn. */
static void ensure_tetra(idx_t maxn)
{
    while (next_tetra_val <= maxn) {
        add_tetra(next_tetra_val);
        ++next_tetra_m;
        idx_t m = next_tetra_m;
        next_tetra_val = m * (m + 1) * (m + 2) / 6;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for 0, but safe guard */

    idx_t kmax = (n + 1) / 2;
    ensure_lucas(kmax);
    ensure_tetra(n);

    seq_t sum = 0;
    for (idx_t i = 0; i < tetra_len; ++i) {
        idx_t t = tetra[i];
        if (t > n)
            break;
        idx_t k = n - t + 1;   /* 1‑based index into Lucas */
        if (k >= 1 && k <= kmax) {
            sum += lucas[k];
        }
    }
    return sum;
}
