/*
 * A033013  Every run of digits of n in base 15 has length 2.
 * https://oeis.org/A033013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 16·s where s is the n‑th positive integer whose base‑225
 *            representation uses only the digits 0…14, the first digit is
 *            non‑zero, and no two consecutive digits are equal
 * Range:     u8 n <= 14, u16 n <= 210, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A033013. */
seq_t a(idx_t n)
{
    /* Base case: for n <= 0 return 0 (offset is 1 in the OEIS). */
    if (n == 0)
        return 0;

    /* Step 1: Find the length k of the base‑225 representation,
       i.e. the smallest k with   Σ_{i=1}^{k} 14^i   ≥ n. */
    seq_t pow14 = 1;         /* 14^0 */
    seq_t cumulative = 0;    /* Σ_{i=1}^{len} 14^i  */
    int k = 0;               /* current length */

    while (cumulative < n) {
        pow14 *= 14;         /* pow14 = 14^k  */
        k++;
        cumulative += pow14;
    }

    /* Now cumulative = 14^1 + ... + 14^k = C(k) */
    seq_t cumulative_prev = cumulative - pow14;   /* C(k‑1) */
    seq_t offset = n - cumulative_prev - 1;       /* 0‑based offset in k‑digit block */

    /* Step 2: Determine each digit d_i.  The digits are chosen
       lexicographically: for position 1, digits 1…14; for i>1,
       digits 0…14 except the previous digit.                */

    /* First digit */
    seq_t powFirst = pow14 / 14;                     /* 14^{k‑1}  */
    uint64_t first_idx = (uint64_t)(offset / powFirst); /* 0…13 */
    uint64_t dprev = first_idx + 1;                 /* 1…14 */

    seq_t s = 0;
    s = s * 225 + dprev;                            /* s = d1           */

    seq_t remaining = offset % powFirst;             /* value for following digits */

    /* Subsequent digits */
    seq_t powCur = powFirst; /* powCur = 14^{k‑1} */
    for (int pos = 2; pos <= k; pos++) {
        powCur /= 14;        /* now powCur = 14^{k‑pos} */

        uint64_t idx_digit = (uint64_t)(remaining / powCur); /* 0…13 */
        remaining %= powCur;

        uint64_t digit = idx_digit;
        if (digit >= dprev) digit++;                /* skip the previous digit */

        s = s * 225 + digit;                        /* extend base‑225 number */

        dprev = digit;                              /* update previous digit */
    }

    /* Step 3: Final answer is 16·s. */
    return s * 16;
}
