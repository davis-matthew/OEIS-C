/*
 * A171834  Partial sums of numbers congruent to {0, 1, 2, 7} mod 8 (A047527).
 * https://oeis.org/A171834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A171834 – partial sums of numbers congruent to {0,1,2,7} mod 8.
 * This function implements the closed‐form:
 *   Let B = n / 4 and r = n % 4.
 *   a(n) = 16*B^2 + 2*B + 8*B*r + S(r)
 *   where S(0)=0, S(1)=1, S(2)=3, S(3)=10.
 * This follows from summing the explicit pattern of the included numbers:
 *   8B+1, 8B+2, 8B+7, 8B+8.
 * The function works for any unsigned seq_t type and handles n ≥ 1.
 */

seq_t a(idx_t n) {
    /* Compute number of complete blocks of 4 terms. */
    idx_t B = n / 4;
    int rem = n % 4;

    /* Convert to seq_t for arithmetic. */
    seq_t B_seq = (seq_t)B;
    seq_t rem_seq = (seq_t)rem;

    /* Sum for all full blocks: 16*B^2 + 2*B. */
    seq_t result = 16 * B_seq * B_seq + 2 * B_seq;

    /* Add partial block: 8*B*rem. */
    result += 8 * B_seq * rem_seq;

    /* Add the offset sum for the remainder terms. */
    switch (rem) {
        case 1: result += 1; break;      /* 8B+1 */
        case 2: result += 3; break;      /* 8B+1 + 8B+2 */
        case 3: result += 10; break;     /* + 8B+7 */
        case 0: break;                   /* no additional terms */
    }

    return result;
}
