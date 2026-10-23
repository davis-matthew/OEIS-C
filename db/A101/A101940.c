/*
 * A101940  Numbers n with omega(n) < omega of 3 nearest larger and 3 nearest
 *          smaller neighbors.
 * https://oeis.org/A101940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   resize_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A101940: Numbers n such that
 *     omega(n) < omega(m) for the 3 integers m < n and the 3 integers m > n.
 * Implemented with an incremental sieve and lazy generation of the first
 * n terms on demand. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/resize_array.h"

/* ---------- global state ---------- */
static seq_t *seq = NULL;          /* array of generated terms          */
static idx_t seq_len = 0;          /* number of terms generated          */
static size_t seq_cap = 0;         /* capacity of seq array              */

static uint16_t *omega = NULL;     /* omega values for integers          */
static size_t sieve_limit = 0;     /* current maximum sieved integer     */
static size_t next_scan = 0;       /* next integer to examine           */

/* ---------- helpers ---------- */
static void ensure_sievesize(size_t new_limit)
{
    if (new_limit <= sieve_limit)
        return;

    /* allocate a fresh omega array */
    uint16_t *new_omega = (uint16_t*)calloc(new_limit + 1, sizeof(uint16_t));
    if (!new_omega)
        return; /* allocation failed – program will not produce correct values */

    /* simple sieve to count distinct prime factors */
    for (size_t p = 2; p <= new_limit; ++p) {
        if (new_omega[p] == 0) {          /* p is prime */
            for (size_t m = p; m <= new_limit; m += p)
                new_omega[m]++;           /* increment distinct prime factor count */
        }
    }

    free(omega);
    omega = new_omega;
    sieve_limit = new_limit;
}

/* initialise state lazily */
static void init_state(void)
{
    if (sieve_limit == 0) {
        /* initial sieving limit – should cover at least the first 24 terms */
        ensure_sievesize(200000);
        next_scan = 4;          /* start scanning after 3 neighbours below 4 */
    }
}

/* generate terms until at least 'want' terms are available */
static void generate_until(idx_t want)
{
    if (seq_len >= want)
        return;

    init_state();

    while (seq_len < want) {
        /* make sure omega is known for the required neighbours */
        if (next_scan + 3 > sieve_limit) {
            size_t newlim = sieve_limit ? sieve_limit * 2 : 200000;
            if (newlim < next_scan + 3)
                newlim = next_scan + 3 + 100;
            ensure_sievesize(newlim);
        }

        if (next_scan >= 4) {
            uint16_t o = omega[next_scan];
            if (o < omega[next_scan - 1] &&
                o < omega[next_scan - 2] &&
                o < omega[next_scan - 3] &&
                o < omega[next_scan + 1] &&
                o < omega[next_scan + 2] &&
                o < omega[next_scan + 3]) {
                /* ensure storage for the new term */
                if (seq_len + 1 > seq_cap) {
                    seq_cap = (seq_cap == 0) ? (size_t)want : seq_cap * 2;
                    seq = resize_array(seq, seq_len, seq_cap);
                }
                seq[seq_len++] = (seq_t)next_scan;
            }
        }
        ++next_scan;
    }
}

/* ---------- required function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1 – caller should not ask for n=0 */
        return 0;
    generate_until(n);
    return seq[n - 1];
}
