/*
 * A079874  Number of configurations in the Classic Lights Out puzzle with n
 *          lights on.
 * https://oeis.org/A079874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of reachable configurations with exactly n lights
 *            * on in the classic 5×5 Lights Out puzzle. The values are the
 *            weight * distribution of the 23‑dimensional binary linear code
 *            of length 25
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   popcount32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/popcount32.h"

/* Count of reachable configurations for each weight (0…25).  The array
 * is zero-initialised at file scope and populated once. */
static seq_t weight_counts[26] = {0};

/* Build the weight distribution of the reachable states of the
 * 5×5 Lights‑Out puzzle.  The routine is idempotent: it runs only once. */
static void init_distribution(void)
{
    static bool initialized = false;
    if (initialized)
        return;

    const int N = 25;                /* number of lights */
    uint32_t press_masks[N];

    /* Pre‑compute the toggle mask for each button. */
    for (int r = 0; r < 5; ++r)
        for (int c = 0; c < 5; ++c)
        {
            int idx = r * 5 + c;
            uint32_t m = 0;
            m |= (1U << idx);                       /* button itself   */
            if (r > 0)   m |= (1U << ((r - 1) * 5 + c));
            if (r < 4)   m |= (1U << ((r + 1) * 5 + c));
            if (c > 0)   m |= (1U << (r * 5 + (c - 1)));
            if (c < 4)   m |= (1U << (r * 5 + (c + 1)));
            press_masks[idx] = m;
        }

    /* Gaussian elimination over GF(2) to find a basis of the column space. */
    uint32_t basis[25] = {0};
    int rank = 0;
    for (int i = 0; i < N; ++i)
    {
        uint32_t x = press_masks[i];
        for (int bit = N - 1; bit >= 0; --bit)
        {
            if (x & (1U << bit))
            {
                if (basis[bit] == 0)
                {
                    basis[bit] = x;
                    ++rank;
                    break;
                }
                else
                    x ^= basis[bit];
            }
        }
    }

    /* Extract the basis vectors into a compact array. */
    uint32_t basis_vec[rank];
    int pos = 0;
    for (int b = 0; b < N; ++b)
        if (basis[b] != 0)
            basis_vec[pos++] = basis[b];

    /* Enumerate all 2^rank reachable states and count their weights. */
    size_t total = ((size_t)1) << rank;
    for (size_t code = 0; code < total; ++code)
    {
        uint32_t state = 0;
        for (int b = 0; b < rank; ++b)
            if (code & ((size_t)1 << b))
                state ^= basis_vec[b];
        unsigned w = popcount32(state);
        weight_counts[w] = weight_counts[w] + (seq_t)1;
    }

    initialized = true;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    init_distribution();
    if (n <= (idx_t)25)
        return weight_counts[(size_t)n];
    return (seq_t)0;
}
