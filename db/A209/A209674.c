/*
 * A209674  For each n, define a sequence of numbers by S(0)=n, S(i) = sum of
 *          last two digits of the concatenation S(0)S(1)S(2)...S(i-1); a(n) =
 *          smallest m such that S(m) = 5, or -1 if 5 is never reached.
 * https://oeis.org/A209674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For a given starting value n, computes the minimum index m such that
   the sequence S satisfies S(0)=n and for i>0,
   S(i)=sum of the last two decimal digits of S(0)S(1)…S(i-1).
   Returns m when S(m)=5, otherwise -1 (this occurs exactly when n ends in 00). */
seq_t a(idx_t n)
{
    /* S(0)=n, so return 0 if already 5. */
    if (n == 5) return 0;

    /* 5 cannot be reached from numbers ending in 00. */
    idx_t last_two = n % 100;
    if (last_two == 0) return (seq_t)-1;

    /* current last two digits of the concatenated string */
    seq_t L = (seq_t)last_two;
    seq_t step = 1;           /* step count m (S(1) is the first computed value) */

    /* We need at most 100 steps (state space size). 200 is a safe bound. */
    for (int i = 0; i < 200; ++i) {
        seq_t d = (L / 10) + (L % 10);    /* digit sum of L */
        if (d == 5) return step;

        /* update last two digits after appending S(i) */
        if (d < 10)
            L = (L * 10 + d) % 100;       /* one extra digit appended */
        else
            L = d;                        /* d is 10–18, so new L is d itself */

        ++step;
    }
    /* Theoretically unreachable for valid inputs; defensive fallback. */
    return (seq_t)-1;
}
