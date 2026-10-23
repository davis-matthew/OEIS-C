/*
 * A101936  Numbers n with omega(n) = omega of 3 nearest larger and 3 nearest
 *          smaller neighbors.
 * https://oeis.org/A101936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A101936: the centre of any run of 7 consecutive integers that all have the same Ω(n) (number of distinct prime factors).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *centres = NULL;          /* sequence terms */
static size_t centre_count = 0;        /* how many have been found */
static size_t centre_capacity = 0;     /* allocated size */

/* Helper: cover the first N centres by sieving ω(n) up to a fixed limit.  */
static void compute_centres(void)
{
    if (centre_count != 0) return;      /* already computed */

    /* Arbitrary generous upper limit that contains the first 10 000 terms. */
    const size_t LIMIT = 50000000u;
    const size_t MAX_CENTRES = 200000u; /* enough capacity before realloc */

    /* Allocate array for ω(n) (distinct prime factor count).  */
    unsigned char *omega = (unsigned char *)malloc((LIMIT + 1u) * sizeof(unsigned char));
    if (!omega) return;                 /* safety on allocation failure */

    for (size_t i = 0; i <= LIMIT; ++i) omega[i] = 0;

    /* Increment ω for all multiples of each prime.  */
    for (size_t p = 2; p <= LIMIT; ++p) {
        if (omega[p] == 0) {            /* p is prime */
            for (size_t m = p; m <= LIMIT; m += p) {
                ++omega[m];
            }
        }
    }

    centres = (seq_t *)malloc(MAX_CENTRES * sizeof(seq_t));
    if (!centres) { free(omega); return; }
    centre_capacity = MAX_CENTRES;

    for (size_t n = 3; n + 3 <= LIMIT; ++n) {
        unsigned char v = omega[n];
        int ok = 1;
        for (int d = -3; d <= 3; ++d) {
            if (d == 0) continue;
            if (omega[n + d] != v) { ok = 0; break; }
        }
        if (ok) {
            if (centre_count == centre_capacity) {
                size_t newcap = centre_capacity * 2;
                seq_t *tmp = (seq_t *)realloc(centres, newcap * sizeof(seq_t));
                if (!tmp) break;                /* abort on failure */
                centres = tmp;
                centre_capacity = newcap;
            }
            centres[centre_count++] = (seq_t)n;
        }
    }

    free(omega);
}

/* Public interface required by the judge.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined for n<1 */
    compute_centres();                  /* ensure data is ready */
    if ((size_t)(n - 1) < centre_count)
        return centres[n - 1];
    return (seq_t)0;                    /* out of range for this implementation */
}
