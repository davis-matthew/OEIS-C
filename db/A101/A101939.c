/*
 * A101939  Numbers n with omega(n) > omega of 3 nearest larger and 3 nearest
 *          smaller neighbors.
 * https://oeis.org/A101939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 5604, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n with ω(n) > ω of the 3 immediate smaller and
   3 immediate larger neighbors.  Computed by incremental search
   using a pre‑computed table of ω(k) (distinct prime factor count). */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Static table of ω values.  arr[i] == number of distinct prime factors of i */
static unsigned char *omega_arr = NULL;
static idx_t omega_limit = 0;          /* current size of omega_arr */

/* Build ω table up to 'limit' (exclusive).  The table is rebuilt from scratch. */
static void build_omega(idx_t limit)
{
    unsigned char *arr = (unsigned char *)calloc(limit, sizeof(unsigned char));
    if (!arr) exit(1);                 /* abort on memory failure */

    for (idx_t p = 2; p < limit; ++p) {
        if (arr[p] == 0) {             /* p is prime */
            for (idx_t j = p; j < limit; j += p)
                ++arr[j];              /* count prime factor p */
        }
    }

    free(omega_arr);
    omega_arr = arr;
    omega_limit = limit;
}

/* Ensure that ω values are available up to at least 'n' (exclusive). */
static void ensure_omega(idx_t n)
{
    if (n < omega_limit) return;

    idx_t new_lim = omega_limit ? omega_limit << 1 : 1024;
    while (new_lim <= n) new_lim <<= 1;

    build_omega(new_lim);
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n = 0 */

    idx_t candidate = 2;                /* start scanning from 2 */
    idx_t found = 0;

    while (1) {
        /* Make sure the ω table covers the candidate and its 3-neighbor range. */
        ensure_omega(candidate + 3);

        int omega_c = omega_arr[(idx_t)candidate];
        bool ok = true;

        /* Check the 3 smaller and the 3 larger neighbors. */
        for (int d = -3; d <= 3; ++d) {
            if (d == 0) continue;
            idx_t neigh = candidate + d;
            if (neigh < 2) continue;          /* skip non‑positive neighbors */
            /* neigh must be < omega_limit because of ensure_omega. */
            int omega_n = omega_arr[(idx_t)neigh];
            if (omega_n >= omega_c) { ok = false; break; }
        }

        if (ok) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
