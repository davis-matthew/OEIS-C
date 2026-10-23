/*
 * A165331  Number of iterations until a fixed point or cycle is reached when
 *          repeatedly replacing a number with the sum of the cubes of its
 *          digits.
 * https://oeis.org/A165331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9798, u32 n <= 9798, u64 n <= 9798,
 *            u128 n <= 9798, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of iterations until a fixed point or cycle is reached
   when repeatedly replacing a number with the sum of the cubes of its decimal digits. */

static seq_t sum_cubes(seq_t v)
{
    seq_t sum = 0;
    while (v > 0)
    {
        seq_t d = v % 10;
        sum += d * d * d;
        v /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t tortoise = sum_cubes(cur);
    seq_t hare = sum_cubes(sum_cubes(cur));

    /* Find a meeting point of tortoise and hare. */
    while (tortoise != hare)
    {
        tortoise = sum_cubes(tortoise);
        hare = sum_cubes(sum_cubes(hare));
    }

    /* Find the start of the cycle (mu). */
    seq_t mu_start = (seq_t)n;
    idx_t mu = 0;
    while (mu_start != tortoise)
    {
        mu_start = sum_cubes(mu_start);
        tortoise = sum_cubes(tortoise);
        mu++;
    }

    return (seq_t)mu;
}
