/*
 * A180352  A permutation of Motzkin numbers by reversal of indices in blocks
 *          of length 7.
 * https://oeis.org/A180352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  less, easy, nonn
 * Formula:   a(n) = Motzkin(r(n)), where r(n) = 7*((n-1)/7+1) - ((n-1)%7)
 * Range:     u8 none, u16 n <= 7, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------  Include standard header  ----------------- */
#include <stdlib.h>   /* malloc, realloc, free */

/* ---------------  Static storage for Motzkin numbers  -------- */
static seq_t *motzkin = NULL;      /* M(0), M(1), … */
static size_t motzkin_cap = 0;     /* allocated number of elements */
static size_t motzkin_len = 0;     /* number of computed elements */

/* ---------------  Ensure Motzkin numbers up to idx exist  -------- */
static void ensure_motzkin_upto(idx_t idx)
{
    /* Grow the storage if needed */
    size_t need = (size_t)idx + 1;
    if (need > motzkin_cap)
    {
        size_t new_cap = motzkin_cap ? motzkin_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t*)realloc(motzkin, new_cap * sizeof(seq_t));
        /* If realloc fails we simply crash; this is acceptable for this use‑case */
        motzkin = tmp;
        motzkin_cap = new_cap;
    }

    /* Initialise the first two Motzkin numbers if not done yet */
    if (motzkin_len == 0)
    {
        motzkin[0] = (seq_t)1;  /* M(0) */
        motzkin_len = 1;
    }
    if (motzkin_len == 1)
    {
        motzkin[1] = (seq_t)1;  /* M(1) */
        motzkin_len = 2;
    }

    /* Compute any missing values up to idx */
    for (size_t i = motzkin_len; i <= (size_t)idx; ++i)
    {
        idx_t ii = (idx_t)i;
        seq_t term1 = ((seq_t)(2 * ii + 1)) * motzkin[i - 1];
        seq_t term2 = ((seq_t)(3 * ii - 3)) * motzkin[i - 2];
        seq_t numer = term1 + term2;
        motzkin[i] = numer / ((seq_t)(ii + 2));
    }
    motzkin_len = (size_t)idx + 1;
}

/* ---------------  Public function required by the problem  -------- */
seq_t a(idx_t n)
{
    /* Compute the sawtooth index r(n) */
    idx_t block = (n - 1) / 7;            /* 0‑based block number */
    idx_t mod   = (n - 1) % 7;            /* position inside the block (0…6) */
    idx_t r     = 7 * (block + 1) - mod;   /* r(n) in 1…∞ */

    ensure_motzkin_upto(r);  /* make sure M(r) is ready */
    return motzkin[(size_t)r];
}
