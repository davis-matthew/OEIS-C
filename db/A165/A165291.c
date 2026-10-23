/*
 * A165291  Complement of A165289.
 * https://oeis.org/A165291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complement of A165289: numbers that cannot be expressed as k^2 - floor(k^(2/3))^3. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Static initialization variables */
    static bool initialized = false;
    static uint8_t *present = NULL;      /* marks representable differences <= LIMIT */
    static seq_t *complements = NULL;    /* array of complement values (1‑based) */
    static size_t comp_size = 0;         /* number of complements stored */
    const size_t LIMIT = 400000;         /* maximum difference to consider */
    const idx_t K_MAX = 12000;           /* maximum k to iterate */

    if (!initialized)
    {
        /* Allocate and initialise presence table */
        present = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
        if (!present) return (seq_t)0; /* safe fallback */

        /* Compute representable differences */
        for (idx_t k = 1; k <= K_MAX; ++k)
        {
            seq_t ksq = (seq_t)k * (seq_t)k;                /* k^2 */
            seq_t m_seq = floor_cube_root(ksq);             /* floor(k^(2/3)) */
            idx_t m = (idx_t)m_seq;                        /* convert for integer ops */
            seq_t cube = (seq_t)m * (seq_t)m * (seq_t)m;    /* m^3 */
            seq_t diff = ksq - cube;                        /* k^2 - m^3 */
            if (diff <= LIMIT)
                present[(size_t)diff] = 1U;
        }

        /* Allocate array for complement values */
        complements = (seq_t *)malloc((LIMIT + 1) * sizeof(seq_t));
        if (!complements) return (seq_t)0; /* safe fallback */

        /* Collect complements (exclude 0 and 1) */
        for (size_t val = 2; val <= LIMIT; ++val)
        {
            if (!present[val])
                complements[comp_size++] = (seq_t)val;
        }

        initialized = true;
    }

    /* Return the requested term if available */
    if (n == 0 || n > comp_size)
        return (seq_t)0;   /* out of bounds request -> 0 */

    return complements[n - 1];  /* offset 1: a(1) is first element */
}
