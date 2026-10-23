/*
 * A109408  Cubes whose digits sum to a prime.
 * https://oeis.org/A109408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   cube, is_prime, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (A076204(n))^3
 * where A076204(n) is the nth positive integer whose cube's decimal
 * digit sum is prime.
 *
 * The function computes the nth cube by scanning integers,
 * checking whether the digit sum of the cube is prime.
 * It uses the utilities cube(), is_prime(), and seq_to_dec_string()
 * to avoid writing arbitrary‑precision arithmetic.
 *
 * Complexity: O(n) on average (density ≈ 1/4), well under 2 s for n ≈ 10 000.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/cube.h"
#include "../utils/seq_to_dec_string.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t x = 1;
    seq_t cube_val;

    /* Buffer large enough for any cube up to 128‑bit (≤39 decimal digits). */
    char buf[256];

    while (1) {
        cube_val = cube((seq_t)x);           /* compute x³ */

        /* Convert cube to decimal string to sum its digits. */
        seq_to_dec_string(cube_val, buf, sizeof buf);

        /* Digit sum of the cube. */
        uint64_t sum = 0;
        for (char *p = buf; *p; ++p)
            sum += (uint8_t)(*p - '0');

        /* Test primality of the digit sum. */
        if (is_prime(sum)) {
            ++count;
            if (count == n)
                return cube_val;
        }

        ++x;
    }
}
