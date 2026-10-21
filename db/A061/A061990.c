/*
 * A061990  Number of ways to place 4 nonattacking queens on a 4 X n board.
 * https://oeis.org/A061990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 18*n^3 + 139*n^2 - 534*n + 840 (for n >= 7)
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 260, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 7 the value is computed by brute force. */

static inline unsigned long long abs_diff(unsigned long long a, unsigned long long b)
{
    return (a > b) ? (a - b) : (b - a);
}

seq_t a(idx_t n)
{
    /* Handle very small boards directly. */
    if (n < 4)  /* a(0) … a(3) = 0 */
        return (seq_t)0;

    /* For n < 7 we enumerate all configurations. */
    if (n < 7) {
        idx_t nn = n;
        seq_t count = 0;

        /* all 24 permutations of rows 0..3 */
        static const int perms[24][4] = {
            {0,1,2,3}, {0,1,3,2}, {0,2,1,3}, {0,2,3,1},
            {0,3,1,2}, {0,3,2,1}, {1,0,2,3}, {1,0,3,2},
            {1,2,0,3}, {1,2,3,0}, {1,3,0,2}, {1,3,2,0},
            {2,0,1,3}, {2,0,3,1}, {2,1,0,3}, {2,1,3,0},
            {2,3,0,1}, {2,3,1,0}, {3,0,1,2}, {3,0,2,1},
            {3,1,0,2}, {3,1,2,0}, {3,2,0,1}, {3,2,1,0}
        };

        for (idx_t c0 = 0; c0 <= nn - 4; ++c0)
            for (idx_t c1 = c0 + 1; c1 <= nn - 3; ++c1)
                for (idx_t c2 = c1 + 1; c2 <= nn - 2; ++c2)
                    for (idx_t c3 = c2 + 1; c3 <= nn - 1; ++c3) {
                        unsigned long long col[4] = {c0, c1, c2, c3};

                        for (int p = 0; p < 24; ++p) {
                            unsigned long long r0 = perms[p][0];
                            unsigned long long r1 = perms[p][1];
                            unsigned long long r2 = perms[p][2];
                            unsigned long long r3 = perms[p][3];

                            /* Check diagonal conflicts. */
                            int ok = 1;
                            if (abs_diff(r0, r1) == abs_diff(col[0], col[1])) ok = 0;
                            else if (abs_diff(r0, r2) == abs_diff(col[0], col[2])) ok = 0;
                            else if (abs_diff(r0, r3) == abs_diff(col[0], col[3])) ok = 0;
                            else if (abs_diff(r1, r2) == abs_diff(col[1], col[2])) ok = 0;
                            else if (abs_diff(r1, r3) == abs_diff(col[1], col[3])) ok = 0;
                            else if (abs_diff(r2, r3) == abs_diff(col[2], col[3])) ok = 0;

                            if (ok) ++count;
                        }
                    }

        return count;
    }

    /* For n >= 7 use the polynomial formula. */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t n4 = n3 * ns;

    seq_t result = n4
                 - (seq_t)18 * n3
                 + (seq_t)139 * n2
                 - (seq_t)534 * ns
                 + (seq_t)840;

    return result;
}
