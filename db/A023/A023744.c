/*
 * A023744  Base 6 expansion uses each positive digit just once.
 * https://oeis.org/A023744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   a(n) is the nth smallest base‑6 number that uses each digit 0‑5
 *            exactly once
 * Range:     u8 none, u16 n <= 720, u32 n <= 720, u64 n <= 720, u128 n <= 720,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the (n-1)th lexicographic permutation of {0,1,2,3,4,5} and interpret it as a base‑6 integer. */

seq_t a(idx_t n)
{
    /* The sequence is defined only for 1 ≤ n ≤ 720; return 0 otherwise. */
    const int LIMIT = 720;               /* 6! */
    if (n == 0 || (n - 1) >= (idx_t)LIMIT)
        return (seq_t)0;

    /* Pre‑computed factorials 0! … 6! */
    static const int fact[7] = {1, 1, 2, 6, 24, 120, 720};

    idx_t k  = n - 1;                     /* zero‑based index */
    int    avail[6] = {0, 1, 2, 3, 4, 5};  /* digits still available */
    int    perm [6];                       /* the chosen permutation */

    for (int i = 0; i < 6; ++i) {
        int f = fact[5 - i];              /* (5-i)! for the remaining positions */
        idx_t pos = k / f;                /* which digit to pick */
        k %= f;

        perm[i] = avail[(int)pos];

        /* Remove that element from the available list */
        for (int j = pos; j < 6 - i - 1; ++j)
            avail[j] = avail[j + 1];
    }

    /* Convert the digit sequence to a base‑6 integer. */
    seq_t value = 0;
    for (int i = 0; i < 6; ++i)
        value = value * (seq_t)6 + (seq_t)perm[i];

    return value;
}
